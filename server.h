#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "Message.h"
#include "MessageBuffer.h"
#include <string>
#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include <sstream>
#include <iostream>
#include <fcntl.h>



using namespace std;

class Server {
public:
    Server();
    ~Server();

    void run();
    void enable_sdebugging();

     
protected:
    virtual void create();
    virtual void close_socket();
    void serve();
    void handle(int, MessageBuffer);
    string get_request(int, MessageBuffer);
    bool send_response(int, string);
    void printqueue(queue<int>);
	vector<Message> messages;

	string mPut(string, MessageBuffer);
	string mReset(MessageBuffer);
	string mList(string, MessageBuffer);
	string mGet(string, MessageBuffer);


    string parse_request(string, int, MessageBuffer);
    string get_longrequest(int, int, MessageBuffer);

    void* thread_execute();
    static void* callExecute(void *arg)
    {
        return ((Server*)arg)->thread_execute(); 

    }
    int sdebugging_flag;
    int server_;
    int buflen_;
    string cache_;
    char* buf_;
    int thread_count;


    sem_t message_lock;    //critical section for modifying message storage
    sem_t queue_lock;      //critical section for modifying threadpool
    sem_t not_full;      //critical section for thread synch
    sem_t not_empty;          //critical section for thread synch

    queue<int> clients;
    queue<MessageBuffer> clients2;
    queue<pthread_t*> threads;
};
