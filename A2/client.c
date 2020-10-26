#include "client.h"


// accept args
// branch input
// branch print
// branch accept
// branch send 

//lists
// * send queue, to send
// * received queue, to print


int main(int argc, char *argv[]){

    if(argc != 4){
        fprintf( stderr, "incorrect number of arguments\n");
        exit(1);
    }

    addresses* addrs = malloc(sizeof(addresses));
    addrs->local_port = strcpy(malloc((ADDRLENGTH+1) * sizeof(char)), argv[1]);
    addrs->remote_name = strcpy(malloc((ADDRLENGTH+1) * sizeof(char)), argv[2]);
    addrs->remote_port = strcpy(malloc((ADDRLENGTH+1) * sizeof(char)), argv[3]);

    pthread_t th1, th2, th3, th4;

    int resp;
    resp = pthread_create( &th1, NULL, reciever, addrs);
    resp = pthread_create( &th2, NULL, sender, addrs);
    resp = pthread_create( &th3, NULL, readinput, addrs);
    resp = pthread_create( &th4, NULL, writeoutput, addrs);

    pthread_exit(NULL);
} 