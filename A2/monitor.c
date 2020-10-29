#include "monitor.h"

void init_monitor()
{

    pthread_mutex_init(&mtxSend, NULL);
    pthread_mutex_init(&mtxReceive, NULL);
    int sem_init(sem_t *sem, int pshared, unsigned int value);
    int sem_init(sem_t *sem, int pshared, unsigned int value);
    sendQueue = List_create();
    receiveQueue = List_create();
}

int send_prepend(void* item) 
{
    pthread_mutex_lock (&mtxSend);
    int ret = List_prepend(sendQueue, item); // perform prepend within mutex
    pthread_mutex_unlock (&mtxSend);
    sem_post(&send_elems);
    return ret;
}

void* send_trim()
{
    sem_wait(&send_elems);
    pthread_mutex_lock (&mtxSend);
    void* item = List_trim(sendQueue); //perform trim within mutex
    pthread_mutex_unlock (&mtxSend);
    return item;
}

int receive_prepend(void* item) 
{
    pthread_mutex_lock (&mtxReceive);
    int ret = List_prepend(receiveQueue, item); // perform prepend within mutex
    pthread_mutex_unlock (&mtxReceive);
    sem_post(&recv_elems);
    return ret;
}

void* receive_trim()
{
    sem_wait(&recv_elems);
    pthread_mutex_lock (&mtxReceive);
    void* item = List_trim(receiveQueue);  //perform trim within mutex
    pthread_mutex_unlock (&mtxReceive);
    return item;
}