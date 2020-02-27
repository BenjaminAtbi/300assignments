#include "client.h"

/*
reciever shit
    * 
    *  getaddrinfo(...)
    *  socket(...)
    *  bind(...)
    *  loop recvfrom(...)
    *  close(...)
*/

/*
sender shit
    *  getaddrinfo(...)
    *  socket(...)
    *  loop sendto(...)
    *  close(...)
*/

/* ************* givens ************** 
    port number
    remote machine name
*/

const char* portnum = "42069";
const char* remoteportnum = "69420";
const char* name = "fakename";


int sender()
{
    //prepare hints 
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    //get target address -> servinfo
    struct addrinfo *servinfo;
    int rv;
    if ((rv = getaddrinfo(name, remoteportnum, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    //make socket
    int sockfd;
    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) != 0) {
        fprintf(stderr, "making socket: $i\n", sockfd);
        return 1;
    }

    return sockfd;
}
