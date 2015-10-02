#include "server.h"
#include <semaphore.h>
#include <stack>
#include <queue>
#include <ostream>

Server::Server() {
    // setup variables
    buflen_ = 1024;
    sdebugging_flag = 0;
    buf_ = new char[buflen_+1];
    thread_count = 10;
}


Server::~Server() 
{
    delete buf_;
}

void
Server::run() {
    // create and run the server
    create();
    serve();
}

void
Server::create() {
}

void
Server::close_socket() {
}

void
Server::serve() {
    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

    sem_init(&queue_lock, 0, 1); // init open
    sem_init(&message_lock,0 , 1); //init locked
    sem_init(&not_full, 0, 0);
    sem_init(&not_empty, 0, thread_count);


    for (int i=0; i<10; i++) 
    {
            pthread_t* thread = new pthread_t;
            pthread_create(thread, NULL, Server::callExecute, this);
            threads.push(thread);
    }

    while (1)
    {
        if((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0)
        {
            sem_wait(&not_empty);
            sem_wait(&queue_lock);
            clients.push(client);
            
            MessageBuffer mb;
            mb.setSocket(client);
            clients2.push(mb);
            
            sem_post(&queue_lock);
            sem_post(&not_full);
        }
    }

    sem_close(&message_lock);
    sem_close(&queue_lock);
    
    close_socket();
}



void* Server::thread_execute()
{
    while(1)
    {
        sem_wait(&not_full);  
        sem_wait(&queue_lock);
        int client = clients.front();        
        clients.pop();
        
        MessageBuffer mb = clients2.front();
        clients2.pop();
        
        
        sem_post(&queue_lock);
        sem_post(&not_empty);
        handle(mb.getSocket(), mb); //thread should die here 
    }

}


void
Server::handle(int client, MessageBuffer mb) 
{
    while (1) 
    {
        string request = get_request(client, mb);
        mb.setRequest(request);

        if (request.empty())
            break;


        string response = parse_request(request, client, mb);

        bool success = send_response(client,response);

        if (not success)
            break;
    }
    close(client);
}


string Server::mReset( MessageBuffer mb)
{
	 sem_wait(&message_lock);
            messages.clear();
     sem_post(&message_lock);
	
	return "OK\n";
}



string
Server::parse_request(string buf_, int client, MessageBuffer mb) 
{
    string request = "";
 //put this whole block of parsing into handle, right before the response is sent.
        stringstream contents(buf_);

        string request_type;
        contents >> request_type;

        if(sdebugging_flag)
        {
           cout << "Request: " << buf_ << endl;
        }


        if(request_type == "reset")
        {
			string resetval = mReset(mb);
            return resetval;
        }

        if(request_type == "put")
        {
            //put(contents.str());
            string name;
            string subject;
            string message;
            string temp = "";
            string length;
            contents >> name;
            contents >> subject;
            contents >> length;

            if(name == "" || subject == "")
            {
                return "error - incorrect put format\n";
            }

            if(!isdigit(length[0]))
            {
                return "error - incorrect put format\n";
            }

            while(contents >> message)
            {
                temp+= " " + message;
            }
            temp.erase(0,1); //erase the first space (laziness)

            int matches = 0; //assign the index
        sem_wait(&message_lock);
            for(int i = 0; i < messages.size(); i++)
            {
                if(messages[i].getName() == name)
                {
                    matches++;
                }
            }
        sem_post(&message_lock);
            int lengthi = atoi(length.c_str());


            if(lengthi > 1023) 
            {
                temp = get_longrequest(client, lengthi);
            }   


            Message m(name,subject, temp, matches + 1);
           // m.toString(); //test to see the contents of the message
            
        sem_wait(&message_lock);
            int before = messages.size();
            messages.push_back(m);
            int after = messages.size();
        sem_post(&message_lock);
            
            if(sdebugging_flag)
            {
                cout << "put: " << name << " " << subject<< " " << "req: " << request << endl;
            }

            if(before == after - 1)
            {
                 request = "OK\n";
            }
            else
            {
                request = "error - not added\n";
            }
            
            return request;
        }

        else if(request_type == "list")
        {
            string name;
            contents >> name;


             if(sdebugging_flag)
            {
                cout << "List request name: " << name << endl;
            }

            if(name == "")
            {
                return "error - no name specified\n";
            }

            stringstream request_ss;

            int matches = 0;
            int found = 0;

        sem_wait(&message_lock);
            for(int i = 0; i < messages.size(); i++)
            {
                if(messages[i].getName() == name)
                {
                    matches ++;
                    request_ss << messages[i].getIndex() << " " << messages[i].getSubject() << "\n";

                }
            }
        sem_post(&message_lock);

            stringstream big_out;
            big_out << "list " << matches << "\n";
            big_out << request_ss.rdbuf();
            request = big_out.str();
           
            return request;
        }

        else if(request_type == "get")
        {

            string name;
            contents >> name;

            string desired_index;
            contents >> desired_index;
            int des_index = atoi(desired_index.c_str());

            if(desired_index == "")
            {
                request = "error - No index specified\n";
            }

        sem_wait(&message_lock);
            for(int i = 0; i < messages.size(); i++)
            {
                if(messages[i].getName() == name && des_index == messages[i].getIndex())
                {
                        stringstream request_ss;
                        request_ss << "message ";
                        request_ss << messages[i].getSubject() << " ";
                        request_ss << messages[i].getMsg().size() << "\n";
                        request_ss << messages[i].getMsg();
                        
                        request = request_ss.str();
        sem_post(&message_lock);
                        return request;
                }
                else
                {
                        request =  "error - Message Not Found\n";
                        //break;
                }
            }

        sem_post(&message_lock);
            return request;
        }

        else if(request_type == "error")
        {
            return buf_;
        }

     return "error - invalid command\n";
}

string
Server::get_request(int client, MessageBuffer mb) 
{
    string request = "";

    while (request.find("\n") == string::npos) 
    {
        int nread = recv(client,buf_,1024,0);
        if (nread < 0) 
        {
            if (errno == EINTR)
                continue;
            else
                return "";
        } 

        else if (nread == 0) 
        {
            return "";
        }
        // be sure to use append in case we have binary data
        request.append(buf_,nread);
    }
    
	cache_ = request;
	string copy = request;
	
    int nlpos = cache_.find("\n");
	int nlpos2 = copy.find("\n");
    cache_.erase(0, nlpos + 1);
	copy.erase(0, nlpos2 + 1);
	
	mb.setCache(cache_);
	
    return request;
}

bool
Server::send_response(int client, string response) {
    // prepare to send response

    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
        //cout << "COULD BE: " << response << endl;

    return true;
}


void Server::enable_sdebugging()
{
    sdebugging_flag = 1;
}

string
Server::get_longrequest(int client, int length) 
{
    // read until we get a newline

    while (cache_.size() < length) {//check size
        int nread = recv(client,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        cache_.append(buf_,nread);
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    //change request to cache

    string request = cache_.substr(0, length + 1);
    cache_.erase(0, length + 1);
    cache_ = "";
    return request;
    //return substring of cache
}


void Server::printqueue(queue<int> clients)
{
    cout <<"QUEUE: " ;
    while(!clients.empty())
    {
        int i;
        i = clients.front();
        cout << i << ", " ;
        clients.pop();
    }
}



