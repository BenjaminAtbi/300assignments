#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#define BUFLENGTH 100

pthread_mutex_t mtxSend;
pthread_mutex_t mtxPrint;
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
