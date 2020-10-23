#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>

#define BUFLENGTH 100

pthread_mutex_t mtxSend;
pthread_mutex_t mtxReceive;
const char* local_port;
const char* remote_name;
const char* remote_port;

void *reciever();
void *sender();
void *get_in_addr(struct sockaddr *sa);

void *writeoutput();
void *readinput();
int readline(char *buf);

#endif
