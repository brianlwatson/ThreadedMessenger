#include "server.h"
#include <semaphore.h>




class QueueHandler
{
	public: 
		QueueHandler()
		{
			 sem_init(&queue_lock,0 , 1); //init locked
		    sem_init(&not_full, 0, 0);
		    sem_init(&not_empty, 0, 10);


		}

		void insertQueue(int client)
		{
			sem_wait(&not_empty);
            sem_wait(&queue_lock);

            MessageBuffer mb;
            mb.setSocket(client);
            clients.push(mb);
            
            sem_post(&queue_lock);
            sem_post(&not_full);
		}


		MessageBuffer popQueue()
		{
			  sem_wait(&not_full);  
		        sem_wait(&queue_lock);
		        
		        MessageBuffer mb = clients.front();
		        clients.pop();
		        
		        sem_post(&queue_lock);
		        sem_post(&not_empty);
		        return mb;
		}

	private:
		queue<MessageBuffer> clients;	
		sem_t queue_lock;      //critical section for modifying threadpool
    	sem_t not_full;      //critical section for thread synch
    	sem_t not_empty; 

};