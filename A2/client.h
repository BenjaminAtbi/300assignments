#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#include "list.h"
#include "monitor.h"

#define MSGLENGTH 100
#define ADDRLENGTH 255

//container for relevant params
typedef struct
{
    const char* local_port;
    const char* remote_name;
    const char* remote_port;
} params;

//thread values
pthread_t* treceiver;
pthread_t* tsender;
pthread_t* treader;
pthread_t* twriter;

//mutex for process exit procedure
static pthread_mutex_t mexit;

//spawned processes
void receiver(const params*);
void sender(const params*);
void reader(const params*);
void writer(const params*);

//ends all threads saftely
void exit_procedure(const params*);
#endif
