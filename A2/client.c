#include "client.h"
#include "list.h"
/* 
    Networking Code referenced from Beej's Guide to Network Programming
    http://beej.us/guide/bgnet/html/#datagram
*/


void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}



/*
    thread that recieves UDP packets
    * pushes packet contents into toPrint list.
    * uses mtxPrint mutex.
*/
void *reciever(void * toPrint)
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
    if ((rv = getaddrinfo(NULL, local_port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
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
        exit(0);
    }

    freeaddrinfo(servinfo);

    //values required for recieving packets
    int numbytes;
    struct sockaddr_storage their_addr;
    socklen_t addr_len = sizeof their_addr;
    
    while(1){

        //recieve a message
        char* buf = malloc(BUFLENGTH * sizeof(char));
        if ((numbytes = recvfrom(sockfd, buf, BUFLENGTH, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            fprintf(stderr, "error reciving\n");
            exit(0);
        }

        pthread_mutex_lock (&mtxPrint);
        //if recive "!", exit
        if(buf[0] == '!'){
            exit(0);
        }

        //push to printlist
        ListPrepend(toPrint, buf);
        pthread_mutex_unlock (&mtxPrint);
    }
}

/*
    thread that sends UDP packets
    * takes strings from toSend list.
    * uses mtxSend mutex.
*/
void *sender(void *toSend)
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
    if ((rv = getaddrinfo(remote_name, remote_port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(0);
    }

    //make socket
    int sockfd;
    struct addrinfo *p;
    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
            fprintf(stderr, "making socket: %i\n", sockfd);
        pthread_exit(NULL);
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            fprintf(stderr, "failed attempt to make socket\n");
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to create outbound socket\n");
        exit(0);
    }

    

    //send packets
    int numbytes;
    while(1){

        pthread_mutex_lock (&mtxSend);
        if(ListCount(toSend) > 0){

            //take string from list, send as packet, then free string.
            char * buf = ListTrim(toSend);
            if ((numbytes = sendto(sockfd, buf, BUFLENGTH, 0,
                p->ai_addr, p->ai_addrlen)) == -1) {
                fprintf(stderr, "Failed to send message\n");
                exit(0);
            }
            if (buf[0] == '!'){
                exit(0);
            }
            free(buf);
        }
        pthread_mutex_unlock (&mtxSend);
    }
}

/*
    thread that recieves input from terminal
    * reads BUFLENGTH lines
    * longer lines split into separate instances
    * puts strings in toSend list.
    * uses mtxSend mutex.
*/
void *readinput(void *toSend) 
{
    
    while(1)
    {
        char* buf = malloc(BUFLENGTH * sizeof(char));
        readline(buf);

        pthread_mutex_lock (&mtxSend);
        ListPrepend(toSend,buf);
        pthread_mutex_unlock (&mtxSend);
    }
}

//reads a single line of input data from terminal
//fill a char buffer terminated by \n
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

/*
    thread that sends msgs to terminal
    * pulls from toPrint list
    * uses mtxPrint mutex
*/
void *writeoutput(void *toPrint)
{
    while(1)
    {
        pthread_mutex_lock (&mtxPrint);
        
        if(ListCount(toPrint) > 0){
            char *buf = ListTrim(toPrint);
            printf("%.*s",BUFLENGTH, buf);
            free(buf);
        }
        
        pthread_mutex_unlock (&mtxPrint);
    }
}

int main(int argc, char *argv[]){

    assert(argc == 4);

    local_port = argv[1]; //42069
    //const char* name1 = "csil-cpu2";
    remote_name =argv[2]; //csil-cpu3
    remote_port = argv[3]; //69420
    
    LIST *toSend = ListCreate();
    LIST *toPrint = ListCreate();
    pthread_t th1, th2, th3, th4;
    
    //initialize mutexes
    pthread_mutex_init(&mtxSend, NULL);
    pthread_mutex_init(&mtxPrint, NULL);

    int resp;
    resp = pthread_create( &th1, NULL, reciever, (void*) toPrint);
    resp = pthread_create( &th2, NULL, sender, (void*) toSend);
    resp = pthread_create( &th3, NULL, readinput, (void*) toSend);
    resp = pthread_create( &th4, NULL, writeoutput, (void*) toPrint);

    pthread_exit(NULL);
}