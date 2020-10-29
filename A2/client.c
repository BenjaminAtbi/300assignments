#include "client.h"

int main(int argc, char *argv[]){

    if(argc != 4){
        fprintf( stderr, "incorrect number of arguments\n");
        exit(1);
    }
    params* addrs = malloc(sizeof(params));
    addrs->local_port = strcpy(malloc((ADDRLENGTH+1) * sizeof(char)), argv[1]);
    addrs->remote_name = strcpy(malloc((ADDRLENGTH+1) * sizeof(char)), argv[2]);
    addrs->remote_port = strcpy(malloc((ADDRLENGTH+1) * sizeof(char)), argv[3]);
    
    //initialize thread id buffers
    treceiver = malloc(sizeof(pthread_t));
    tsender = malloc(sizeof(pthread_t));
    treader = malloc(sizeof(pthread_t));
    twriter = malloc(sizeof(pthread_t));

    //initialize shared lists
    init_monitor();

    //initialize exit mutex
    pthread_mutex_init(&mexit, NULL);

    //create threads
    int resp;
    resp = pthread_create( treceiver, NULL, (void *) &receiver, addrs);
    resp = pthread_create( tsender, NULL, (void *) &sender, addrs);
    resp = pthread_create( treader, NULL, (void *) &reader, addrs);
    resp = pthread_create( twriter, NULL, (void *) &writer, addrs);

    pthread_exit(NULL);
} 

//end all threads after their current work is complete
//called by any thread when they reach error condition or exit message
void exit_procedure(const params* addrs)
{
    //prevent multiple threads from attempting to cancel each other before procedure complete
    pthread_mutex_lock(&mexit);  
    //kill receiver and reader if not self
    if(*treceiver != pthread_self()){   
        pthread_cancel(*treceiver);
    }
    if(*treader != pthread_self()){
        pthread_cancel(*treader);
    }

    //send exit message to sender and writer, to be executed upon after their queue is emptied
    char* buf1 = malloc(MSGLENGTH * sizeof(char));
    buf1[0] = '!';
    send_prepend(buf1);

    char* buf2 = malloc(MSGLENGTH * sizeof(char));
    buf2[0] = '!';
    receive_prepend(buf1);
    pthread_mutex_unlock (&mexit);

    //commit suicide. you will be remembered, brave thread 07 07 07
    pthread_exit(NULL);
}