#include "client.h"
#include "list.h"

// accept args
// branch input
// branch print
// branch accept
// branch send 

//lists
// * send queue, to send
// * received queue, to print


int main(int argc, char *argv[]){

    // assert(argc == 4);
    // local_port = argv[1];
    // remote_name =argv[2];
    // remote_port = argv[3];
    
    // LIST *sendQueue = ListCreate();
    // LIST *receiveQueue = ListCreate();
    // pthread_t th1, th2, th3, th4;

        
    // //initialize mutexes
    // pthread_mutex_init(&mtxSend, NULL);
    // pthread_mutex_init(&mtxReceive, NULL);

    // int resp;
    // resp = pthread_create( &th1, NULL, reciever, (void*) toPrint);
    // resp = pthread_create( &th2, NULL, sender, (void*) toSend);
    // resp = pthread_create( &th3, NULL, readinput, (void*) toSend);
    // resp = pthread_create( &th4, NULL, writeoutput, (void*) toPrint);

    // pthread_exit(NULL);

    List *testlist = ListCreate();
    int i = 3;
    List_append(&testlist, &i);
    int j = *(int *) List_remove(&testlist);
    printf("%i", j);
} 