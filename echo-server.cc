#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "inet-server.h"

using namespace std;

int
main(int argc, char **argv)
{
    int option, port;

    // setup default arguments
    port = 3000;
    int debug = 0;
    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc,argv,"p:d")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'd':
                debug = 1;
                break;
            default:
                cout << "server [-p port]" << endl;
                exit(EXIT_FAILURE);
        }
    }

    InetServer server = InetServer(port);
    cout << "Starting Server -p[" << port << "] -d[" << debug << "]" << endl; 

    if(debug == 1)
    {
        server.enable_sdebugging();
    }

    server.run();
}
