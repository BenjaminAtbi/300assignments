#include "sim.h"
#include <limits.h>

/*
    initialize simulation
     - create init process
     - create process queues
     - create message queue
*/


void Initialize()
{
    //create init process
    init = malloc(sizeof(PCB));
    init->priority = -1;
    init->PID =  0;
    memset(init->msg, 0, MSGLENGTH * sizeof(char));
    current = init;

    //instantiate queues and semaphores
    for(int i = 0; i < NUM_QUEUES; i++)
    {
        queues[i] = List_create();
    }

    sendQueue = List_create();
    recvQueue = List_create();
    messages = List_create();

    for(int i = 0; i < NUM_SEMS; i++)
    {
        semaphores[i] = (semaphore) {false, 0, NULL};
    }
    
    pid_counter = 0;
}

/*
    set message field of a process
*/
void setMessage(PCB* process, char* msg)
{
    memcpy(process->msg, msg, MSGLENGTH * sizeof(char));
}

int genPID()
{
    pid_counter++;
    return pid_counter;
}

/*
    updates simulation after change to processes. may set new current function, or end simulation
    - should be ran after any function that alters process states
*/
void updateSim()
{
    //recheck for new process if init is current
    if(current == &init)
    {
        current = NULL;
    }

    // if no process is running, take process from highest prority queue that contains a process
    if(current == NULL)
    {
        for(int i = 0; i < NUM_QUEUES; i++)
        {
            if(List_count(queues[i]) > 0)
            {
                current = List_trim(queues[i]);
                break;
            }
        }
    }

    //if no process found in queues, default to init
    if(current == NULL)
    {

    }
}

/*
    print all info about a referenced PCB
*/

void printPCB(PCBref proc)
{
    printf("#### Process ID: %i ####\n", proc.pcb->PID);
    printf("priority: %i\n",proc.pcb->priority);
    printf("message: %s\n", proc.pcb->msg);
    printf("state: ");
    switch(proc.state)
    {
        case PRIO_0: printf("in priority 0 queue\n"); break;
        case PRIO_1: printf("in priority 1 queue\n"); break;
        case PRIO_2: printf("in priority 2 queue\n"); break;
        case SEND: printf("in send block queue\n"); break;
        case RECV: printf("in receive block queue\n"); break;
        case SEM_0: printf("in semaphore 0 block queue\n"); break;
        case SEM_1: printf("in semaphore 1 block queue\n"); break;
        case SEM_2: printf("in semaphore 2 block queue\n"); break;
        case SEM_3: printf("in semaphore 3 block queue\n"); break;
        case SEM_4: printf("in semaphore 4 block queue\n"); break;
        case RUNNING: printf("currently running\n"); break;
        case INIT: printf("is the Init process\n"); 
    }
    printf("####################\n");
}