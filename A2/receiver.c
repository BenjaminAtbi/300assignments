#include "client.h"

void receiver(const params* addrs)
{
    struct addrinfo hints, *receiver_info, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_DGRAM; //specify UDP
	hints.ai_flags = AI_PASSIVE; //specify address as local IP
    int rv; //return value
    int sockfd; //socket file descriptor
    int numbytes; //actual number of bytes received in a packet
    struct sockaddr_storage their_addr; //source address of received packet
    socklen_t addr_len = sizeof their_addr;

    //get parameters for receiving socket
    if ((rv = getaddrinfo(NULL, addrs->local_port, &hints, &receiver_info)) != 0) 
    {
        fprintf(stderr, "Receiver: getaddrinfo error: %s. exiting\n", gai_strerror(rv));
        exit_procedure(addrs);
    }

    //bind to first valid parameter set
    for(p = receiver_info; p != NULL; p = p->ai_next) 
    {
        //get socket descriptor
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            fprintf(stderr, "Receiver: failed to generate a socket descriptor\n");
            continue;
        }

        //bind parameters to socket
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            fprintf(stderr, "Receiver: failed to bind a socket\n");
            continue;
        }

        break; //move to next step if process successful 
    }

    //ensure socket was created
    if (p == NULL) {
        fprintf(stderr, "Receiver: failed to initialize socket. exiting\n");
        exit_procedure(addrs);
    }

    freeaddrinfo(receiver_info); //we are done with you. Begone.

    // receive messages
    while(1){

        char* buf = malloc(MSGLENGTH * sizeof(char));
        if ((numbytes = recvfrom(sockfd, buf, MSGLENGTH, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            fprintf(stderr, "Receiver: error receiving message.\n");
            exit_procedure(addrs);
        }

        receive_prepend(buf);
        
        //if receive exit msg exit
        if(buf[0] == '!')
        {
            exit_procedure(addrs);
        }
    }
}