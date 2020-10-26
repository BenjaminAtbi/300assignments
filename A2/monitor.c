#include "monitor.h"

void init_monitor()
{
    pthread_mutex_init(&mtxSend, NULL);
    pthread_mutex_init(&mtxReceive, NULL);
    sendQueue = List_create();
    receiveQueue = List_create();
}

int send_prepend(void* item) 
{
    pthread_mutex_lock (&mtxSend);
    int ret = List_prepend(sendQueue, item);
    pthread_mutex_unlock (&mtxSend);
    return ret;
}

void* send_trim()
{
    pthread_mutex_lock (&mtxSend);
    void* item = List_trim(sendQueue);
    pthread_mutex_unlock (&mtxSend);
    return item;
}

int receive_prepend(void* item) 
{
    pthread_mutex_lock (&mtxReceive);
    int ret = List_prepend(receiveQueue, item);
    pthread_mutex_unlock (&mtxReceive);
    return ret;
}

void* receive_trim()
{
    pthread_mutex_lock (&mtxReceive);
    void* item = List_trim(receiveQueue);
    pthread_mutex_unlock (&mtxReceive);
    return item;
}