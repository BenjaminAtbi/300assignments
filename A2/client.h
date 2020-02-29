#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define BUFLENGTH 100

int reciever();
int sender();
void *get_in_addr(struct sockaddr *sa);

int writeoutput();
int readinput();
int readline(char *buf);

#endif