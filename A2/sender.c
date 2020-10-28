#include "client.h"


void sender(addresses addrs)
{
    struct addrinfo hints, *remote_info, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_DGRAM; //specify UDP
    int rv; //return value
    int sockfd; //socket file descriptor

    //get parameters for receiving socket on remote
    if ((rv = getaddrinfo(addrs.remote_name, addrs.remote_port, &hints, &remote_info)) != 0) 
    {
        fprintf(stderr, "Sender getaddrinfo error: %s. exiting\n", gai_strerror(rv));
        exit(1);
    }

    //bind to first valid parameter set
    for(p = remote_info; p != NULL; p = p->ai_next) 
    {
        //get socket descriptor
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            fprintf(stderr, "Sender failed to generate a socket descriptor, continuing \n");
            continue;
        }
        break; //move to next step if process successful 
    }


    //ensure socket was created 
    if (p == NULL) {
    fprintf(stderr, "Sender failed to initialize socket\n");
    exit(0);
    }

    freeaddrinfo(remote_info); //we are done with you. Begone.

    // send messages!
}