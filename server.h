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
#include <string>

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
    void handle(int);
    string get_request(int);
    bool send_response(int, string);

	vector<Message> messages;

    string parse_request(string);
    
    int sdebugging_flag;
    int server_;
    int buflen_;
    string cache_;
    char* buf_;
};
