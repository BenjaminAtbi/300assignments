#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>

#include "list.h"
#include "monitor.h"

#define MSGLENGTH 100
#define ADDRLENGTH 255

//container for relevant addresses
typedef struct
{
    const char* local_port;
    const char* remote_name;
    const char* remote_port;
} addresses;

void *reciever();
void *sender();
void *readinput();
void *writeoutput();

void *get_in_addr(struct sockaddr *sa);
int readline(char *buf);

#endif
