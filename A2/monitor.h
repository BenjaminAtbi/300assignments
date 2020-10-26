#ifndef MONITOR_H
#define MONITOR_H

#include <pthread.h>
#include "list.h"

// perhaps not technically a monitor. Manages access to two queues under separate mutexes.

static pthread_mutex_t mtxSend;
static pthread_mutex_t mtxReceive;
static List *sendQueue;
static List *receiveQueue;

//initialize mutexes and queues
void init_monitor();

//modify send queue
int send_prepend(void* item);
void* send_trim();

//modify receive queue
int receive_prepend(void* item);
void* receive_trim();

#endif
