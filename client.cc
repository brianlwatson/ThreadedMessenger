#include "client.h"
#include <sstream>
#include <string>
using namespace std;

Client::Client() {
    // setup variables
    buflen_ = 1024 * 10;
    buf_ = new char[buflen_+1];
    debugging_flag = 0;
}

Client::~Client() {
}

void Client::run() {
    // connect to the server and run echo program
    create();
    echo();
}

void
Client::create() {
}

void
Client::close_socket() {
}

void
Client::echo() {
    string line;
    
    // loop to handle user interface
    cout << "% ";
    while (getline(cin,line)) 
    {
      line = parse_request(line);

      if(debugging_flag == 1)
      {
          cout << "Parsed Request: " << line << endl;
      }

      if(line == ("**QUIT**!@#$")) //if entered code is quit
      {
        close_socket();
        return;
      }

      if(line != "I don't recognize that command\n")
      {
        // append a newline
       // line += "\n";
        // send request
        bool success = send_request(line);


        // break if an error occurred
        if (not success)
            break;
        // get a response


        success = get_response();             //THIS IS VITAL
        // break if an error occurred


        if (not success)
            break;
        }

        else
        {
            cout << line;
        }

        cout << "% ";
    }

    close_socket();
}

bool
Client::send_request(string request) {
    // prepare to send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
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
    return true;
}

bool
Client::get_response() {
    string response = "";
    // read until we get a newline
    while (response.find("\n") == string::npos) {

        int nread = recv(server_,buf_,1024 * 10,0);
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
        response.append(buf_,nread);
    }
    // a better client would cut off anything after the newline and
    // save it in a cache
    
   
    parse_response(response);
    //cout << response;
    return true;
}

string Client::parse_request(string temp)
{
    string toret;

    istringstream ss(temp); 
    stringstream oss;
    string type, user, subject;
    
    ss >> type; //parse out first element of getline

    //first handle QUIT

    if(type == "reset" && debugging_flag == 1)
    {
        return "reset\n";
    }

    if(type == "quit" || type == "QUIT" || type == "quit\n")
    {
        return "**QUIT**!@#$";
    }

    //SEND
    if(type == "send" || type == "SEND")
    {
        if(!(ss >> user))
        {
            return "error - No Defined User\n";
        }

        if(!(ss >> subject))
        {
            return "error - Missing Subject\n";
        }

        string x;
        ss >> x;

        if(x != "")
        {
            return "error - Too Many Send Arguments\n";
        }

        cout << "- Type your message. End with a blank line -\n";

        stringstream message;
        string addmsg;
        do
        {
            getline(cin, addmsg);
            message << addmsg;
        }
        while(addmsg != "");

        oss << "put " << user << " " << subject << " " << message.str().size() << "\n" << message.str();
        return oss.str();
    }

    //list command
    if(type == "list")
    {
        if(!(ss >> user))
        {
            return "error - No Defined User\n";
        }


        string x;
        ss >> x;
        if(x != "")
        {
            return "error - Too Many List Arguments\n";
        }
        oss << "list" << " " << user << "\n";

        return oss.str();
    }

    //read command. translates to get
    if(type == "read")
    {
        if(!(ss >> user))
        {
            return "error - No Defined User\n";
        }

        string index;
        ss >> index;

        string x;
        ss >> x;
        if(x != "")
        {
            return "error - Too Many Read Arguments\n";
        }

        oss << "get" << " " << user << " " << index << "\n";
        return oss.str();
    }

        return "I don't recognize that command\n";    
    
}


string Client::parse_response(string resp)
{

    istringstream iss(resp);

    string parsed;

    string type;
    iss >> type;
    if(type == "OK") //result of a valid send.
    {
        parsed = "";
    }

    if(type == "error")
    {
        cout << resp;
        return resp; //return original string if error.
    }

    if(type == "list")
    {
        int index = resp.find("\n");
         parsed = resp.erase(0,index + 1);
       
    }

    if(type == "message")
    {
          int index = resp.find("\n");
         parsed = resp.erase(0,index + 1);
         parsed += "\n";
    }

    cout << parsed;
 
    return parsed;

}

void Client::enable_debugging()
{
    debugging_flag = 1;
}
