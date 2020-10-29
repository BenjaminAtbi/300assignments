#ifndef MONITOR_H
#define MONITOR_H

#include <pthread.h>
#include <semaphore.h>
#include "list.h"
#include <stdio.h>

// perhaps not technically a monitor, as there is no limit on number of threads accessing functions, or conception of being "within monitor"
// Manages access to two queues under separate mutexes and semaphores.

static pthread_mutex_t mtxSend;
static pthread_mutex_t mtxReceive;
static sem_t send_elems;
static sem_t recv_elems;
static List *sendQueue;
static List *receiveQueue;

//initialize mutexes and queues
void init_monitor();

//concurrency safe prepend
int send_prepend(void* item);

//concurrency safe trim
//locks until element available 
void* send_trim();

//modify receive queue
int receive_prepend(void* item);

//concurrency safe prepend
//locks until element available 
void* receive_trim();

#endif
