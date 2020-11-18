#ifndef SIM_H
#define SIM_H
#include "list.h"

#define MSGLENGTH 40
#define NUM_FLAGS 7
#define NUM_SEMS 5

//declare 4 process queues and message list
//blocked queue defined as last queue
#define NUM_QUEUES 4
#define BLOCKED_QUEUE NUM_QUEUES
List* queues[NUM_QUEUES];
List* messages;


enum pcb_state{ PRIO_0, PRIO_1, PRIO_2, BLOCKED, RUNNING, INIT};
enum pcb_flags{ sem_0, sem_1, sem_2,sem_3,sem_4,send_flag,recv_flag};

// Process Control Block, 
typedef struct PCB
{
    int priority;
    int PID;
    char msg[MSGLENGTH];
    int flags[NUM_FLAGS];
} PCB;


// structure message, specifying both sender and receiver
typedef struct message
{
    char msg[MSGLENGTH];
    int receiver_PID;
    int sender_PID;
} message;

//declare 5 semaphores
int semaphore[NUM_SEMS];
int sem_init[NUM_SEMS];

//counter for unique pids
int pid_counter;

//init process 
PCB init;

//ref to current process
PCB* current;

/* 
   ######################
   ## Helper Functions ##
   ###################### 
*/

void Initialize();
void set_message(PCB process, char* msg);


void Create();
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
void ProcInfo();
void TotalInfo();

#endif