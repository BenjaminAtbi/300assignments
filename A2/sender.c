#include "client.h"

void sender(params *addrs)
{

    struct addrinfo hints;
    struct addrinfo *remote_info, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_DGRAM; //specify UDP
    int rv; //return value
    int sockfd; //socket file descriptor
    int numbytes; //actual number of bytes sent in a packet

    //get parameters for receiving socket on remote
    if ((rv = getaddrinfo(addrs->remote_name, addrs->remote_port, &hints, &remote_info)) != 0) 
    {
        fprintf(stderr, "Sender: getaddrinfo error: %s. exiting\n", gai_strerror(rv));
        exit_procedure(addrs);
    }

    //bind to first valid parameter set
    for(p = remote_info; p != NULL; p = p->ai_next) 
    {
        //get socket descriptor
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            fprintf(stderr, "Sender: failed to generate a socket descriptor, continuing \n");
            continue;
        }
        break; //move to next step if process successful 
    }

    //ensure socket was created 
    if (p == NULL) {
        fprintf(stderr, "Sender: failed to initialize socket\n");
        exit_procedure(addrs);
    }

    freeaddrinfo(remote_info); //we are done with you. Begone.

    while(1){
        char *buf = send_trim();
        //send message
        if ((numbytes = sendto(sockfd, buf, MSGLENGTH, 0, p->ai_addr, p->ai_addrlen)) == -1) {
            fprintf(stderr, "Sender: Failed to send message. exiting\n");
            exit_procedure(addrs);
        }

        //if msg was an exit message, exit
        if(buf[0] == '!'){
            free(buf);
            pthread_exit(NULL);
        }

        free(buf);
    }
}
