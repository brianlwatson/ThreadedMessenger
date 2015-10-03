#include "server.h"
#include <semaphore.h>




class QueueHandler
{
	public: 
		QueueHandler()
		{
			cout <<"INITED" << endl;
			sem_init(&queue_lock, 0, 1); // init open
		   	 sem_init(&not_full, 0, 0);
			 sem_init(&not_empty, 0, 10);
		}
			


		void insertQueue(int client)
		{
			sem_wait(&not_empty);
            sem_wait(&queue_lock);

            ClientHandler mb;
            mb.setSocket(client);
            clients.push(mb);
            
            sem_post(&queue_lock);
            sem_post(&not_full);
		}

		int size()
		{
			return clients.size();
		}


		ClientHandler popQueue()
		{
			    sem_wait(&not_full);  
		        sem_wait(&queue_lock);
		        
		        ClientHandler mb = clients.front();
		        clients.pop();
		        
		        sem_post(&queue_lock);
		        sem_post(&not_empty);
		        
		        return mb;
		}

	private:
		queue<ClientHandler> clients;	
		sem_t queue_lock;      //critical section for modifying threadpool
    	sem_t not_full;      //critical section for thread synch
    	sem_t not_empty; 

};
