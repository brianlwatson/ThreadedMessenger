#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "inet-client.h"

using namespace std;

int
main(int argc, char **argv)
{
    int option;

    // setup default arguments
    int port = 3000;
    string host = "localhost";
    int debug = 0;
    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc,argv,"s:p:d")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                host = optarg;
                break;
            case 'd':
                debug = 1;
                break;
            default:
                cout << "client [-h host] [-p port]" << endl;
                exit(EXIT_FAILURE);
        }
    }

    InetClient client = InetClient(host, port);
    
    if(debug == 1)
    {
        client.enable_debugging();
    }
    
    cout << "Starting Client -p[" << port << "] -h[" << host << "] -d [" << debug << "]" << endl; 

    client.run();
}

