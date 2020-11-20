#ifndef SIM_H
#define SIM_H
#include "list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define MSGLENGTH 40
#define NUM_SEMS 5
#define NUM_QUEUES 3

//declare 3 prio queues, a send block queue, and a receive block queue
//declare a message queue
//blocked queue defined as last queue
List* queues[NUM_QUEUES];
List* sendQueue;
List* recvQueue;
List* messages;


enum pcb_state{ PRIO_0, PRIO_1, PRIO_2, SEND, RECV, SEM_0, SEM_1, SEM_2,SEM_3, SEM_4, RUNNING, INIT};

// Process Control Block, 
typedef struct PCB
{
    int priority;
    int PID;
    char msg[MSGLENGTH];
} PCB;

//reference to a PCB, specifying state of the PCB at time of reference
typedef struct PCBref
{
    PCB* pcb;
    int state;
} PCBref;

// structure message, specifying both sender and receiver
typedef struct message
{
    char msg[MSGLENGTH];
    int receiver_PID;
    int sender_PID;
} message;

// semaphore struct, specifying value and list of blocked processes
typedef struct semaphore
{
    bool init;
    int counter;
    List* blocked_queue;
} semaphore;

//declare 5 semaphores
semaphore semaphores[NUM_SEMS];

//counter for unique pids
int pid_counter;

//ref to init process 
PCB* init;

//ref to current process
PCB* current;

/* 
    ######################
    ## Helper Functions ##
    ###################### 
*/


void Initialize();
int genPID();

void setMessage(PCB* process, char* msg);
PCBref getPCBbyPID(int pid);
void printPCB(PCBref process);

/*
    ##########################
    ## Simulation Functions ##
    ##########################
*/

int Create(int priority);
void Fork();
void Kill();
void Exit();
void Quantum();
void Send();
void Receive();
void Reply();
void NewSem();
void SemP();
void SemV();
void ProcInfo(int pid);
void TotalInfo();

#endif