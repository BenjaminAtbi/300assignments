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

const char* portnum1 = "42069";
const char* portnum2 = "69420";
const char* name1 = "csil-cpu2";
const char* name2 = "csil-cpu3";

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int reciever()
{
//prepare hints 
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; //IP setup

    //get target address -> servinfo
    struct addrinfo *servinfo;
    int rv;
    if ((rv = getaddrinfo(NULL, portnum2, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    int sockfd;
    struct addrinfo *p;
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            fprintf(stderr, "failed to make socket\n");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            fprintf(stderr, "failed to bind\n");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 1;
    }

    freeaddrinfo(servinfo);
    printf("created reciving socket\n");

    //recieve data from addr
    int MAXBUFLEN = 100;
    char buf[MAXBUFLEN];
    int numbytes;
    struct sockaddr_storage their_addr;
    socklen_t addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        fprintf(stderr, "error reciving\n");
        return 1;
    }

    //print source data
    char s[INET6_ADDRSTRLEN];
    printf("listener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
    s, sizeof s));

    //print message
    buf[numbytes] = '\0';
    printf("message: \"%s\"\n", buf);

    close(sockfd);

    return 0;
}

int sender()
{
    //prepare hints 
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
    
    //get target address -> servinfo
    struct addrinfo *servinfo;
    int rv;
    if ((rv = getaddrinfo(name2, portnum2, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    //make socket
    int sockfd;
    struct addrinfo *p;
    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
            fprintf(stderr, "making socket: %i\n", sockfd);
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            fprintf(stderr, "failed attempt to make socket\n");
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to create outbound socket\n");
        return 1;
    }

    char * buf = "u mum gay";
    int numbytes;
    if ((numbytes = sendto(sockfd, buf, strlen(buf), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        fprintf(stderr, "Failed to send message\n");
        return 1;
    }

    freeaddrinfo(servinfo);

    printf("sent %d bytes to %s\n", numbytes, name2);
    close(sockfd);
    return 0;
}


int readinput() 
{
    char buf[BUFLENGTH];
    while(1)
    {
        readline(buf);
        printf("%s",buf);
    }
}

int readline(char *buf)
{
    memset(buf, 0, BUFLENGTH * sizeof(char));
    int i = 0;
    char c = '0';
    while(c != '\n')
    {
        c = getchar();
        buf[i] = c;

        if(i < BUFLENGTH-2)
        {
            i++;   
        } else {
            buf[BUFLENGTH-1] = '\n';
            break;
        }
    }
}

int writeoutput()
{
    while(1)
    {
        sleep(3);
        printf("dumdum\n");
    }
}

threadtest