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
        semaphores[i].blocked_queue = List_create();
    }

    pid_counter = 0;
}

/*
    updates simulation after change to processes. may set new current function, or end simulation
    - should be ran after any function that alters process states
*/
void updateSim()
{
    //recheck for new process if init is current
    if(current == init)
    {
        if(current->msg[0] != 0){
            printf("msg: %s\n",current->msg);
            memset(current->msg, 0, MSGLENGTH * sizeof(char));
        }
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
                printf("process with PID %i now running. (selected from priority %i queue)\n", current->PID, i);
                if(current->msg[0] != 0){
                    printf("msg: %s\n",current->msg);
                    memset(current->msg, 0, MSGLENGTH * sizeof(char));
                }
                break;
            }
        }
    }

    //if no process found in queues, default to init
    if(current == NULL)
    {
        current = init;
        printf("init process now running\n");
    }
    //if no init, exit
    if(current == NULL)
    {
        exitSim();
    }
}

int killProcess(const PCBref process)
{
        if(process.pcb == init)
        {
            //find if another pcbs exists anywhere in simulation 
            bool pcb_exists = false;
            if(current != init && current != NULL) pcb_exists = true;
            pcb_exists = pcb_exists || List_count(sendQueue) > 0;
            pcb_exists = pcb_exists || List_count(recvQueue) > 0;
            for(int i = 0; i < NUM_QUEUES; i++)
            {
                pcb_exists = pcb_exists || List_count(queues[i]) > 0;
            }
            for(int i = 0; i < NUM_SEMS; i++)
            {
                pcb_exists = pcb_exists || (semaphores[i].init && List_count(semaphores[i].blocked_queue) > 0);
            } 
            if(pcb_exists){
                printf("can't kill init process while another process exists in simulation\n");
                return 0;
            } else {
                printf("killed init process\n");
                free(init); 
                exitSim();
                return 1;
            }
        }

        List* host_queue;
        PCB* item;
        switch(process.state)
        {
            case RUNNING: 
                printf("killed currently running process (PID: %i)\n",current->PID);
                free(current);
                current = NULL; 
                updateSim();
                return 1;
            case SEND:
                List_first(sendQueue);
                List_search(sendQueue, &COMPARATOR_PCB_PID, &process.pcb->PID);
                item = List_remove(sendQueue);
                printf("killed process in send block queue (PID %i)\n",item->PID);
                free(item);
                updateSim();
                return 1;
            case RECV:
                List_first(recvQueue);
                List_search(recvQueue, &COMPARATOR_PCB_PID, &process.pcb->PID);
                item = List_remove(recvQueue);
                printf("killed process in recv block queue (PID %i)\n",item->PID);
                free(item);
                updateSim();
                return 1;
            case PRIO_0:
            case PRIO_1:
            case PRIO_2:
                host_queue = queues[process.state - PRIO_0];
                List_first(host_queue);
                List_search(host_queue, &COMPARATOR_PCB_PID, &process.pcb->PID);
                item = List_remove(host_queue);
                printf("killed process in priority %i queue (PID %i)\n",process.state - PRIO_0, item->PID);
                free(item);
                updateSim();
                return 1;
            case SEM_0:
            case SEM_1:
            case SEM_2:
            case SEM_3:
            case SEM_4:
                host_queue = semaphores[process.state - SEM_0].blocked_queue;
                List_first(host_queue);
                List_search(host_queue, &COMPARATOR_PCB_PID, &process.pcb->PID);
                item = List_remove(host_queue);
                printf("killed process in semaphore %i block queue (PID %i)\n",process.state - SEM_0, item->PID);
                free(item);
                updateSim();
                return 1;
            case INIT:
                printf("killed init process\n");
                free(init); 
                exitSim();
                return 1;
        }
}

//comparator between pcb and a pid
bool COMPARATOR_PCB_PID(void* pcb, void* pid)
{
    PCB* ref = (PCB*)pcb;
    return ref->PID == *(int*)pid;
}

//comparator between pcb and a pid
bool COMPARATOR_MSG_RECVR(void* msg, void* pid)
{
    message* ref = (message*)msg;
    return ref->receiver_PID == *(int*)pid;
}

void enqueueProcess(List* queue, PCB* process)
{
    if(process->PID != 0)
    {
        List_prepend(queue, process);
    }
}

/*
    exit simulation
*/
void exitSim()
{
    printf("exiting simulation\n");
    exit(0);
}

/*
    get a new unique PID
*/
int genPID()
{
    pid_counter++;
    return pid_counter;
}

//make reference to a PCB
PCBref makeRef(PCB* process, int state)
{
    return (PCBref) {process, state};
}

//allocate a new message 
message* makeMessage(int receiver, int sender, char* msg)
{
    message* new_message = malloc(sizeof(PCB));
    memcpy(new_message->msg, msg, MSGLENGTH * sizeof(char));
    new_message->receiver_PID = receiver;
    new_message->sender_PID = sender;
    return new_message;
}

/*    set message field of a process
*/
void setMessage(PCB* process, char* msg)
{
    memcpy(process->msg, msg, MSGLENGTH * sizeof(char));
}

/*
    find a PCB identified by its PID
*/
PCBref getPCBbyPID(int pid)
{
    if(pid == 0 && init != NULL)
    {
        return makeRef(init, INIT);
    }

    if(current != NULL && current->PID == pid)
    {
        return makeRef(current, RUNNING);
    }

    //check send and recv queues
    if(List_count(sendQueue) > 0)
    {
        List_first(sendQueue);
        PCB* ref = List_search(sendQueue, &COMPARATOR_PCB_PID, &pid);
        if(ref != NULL)
        {
            return makeRef(List_curr(sendQueue), SEND);
        }
    }

    if(List_count(recvQueue) > 0)
    {
        List_first(recvQueue);
        PCB* ref = List_search(recvQueue, &COMPARATOR_PCB_PID, &pid);
        if(ref != NULL)
        {
            return makeRef(List_curr(recvQueue), RECV);
        }
    }
    for(int i = 0; i < NUM_QUEUES; i++)
    {
        if(List_count(queues[i]) > 0)
        {
            List_first(queues[i]);
            PCB* ref = List_search(queues[i], &COMPARATOR_PCB_PID, &pid);
            if(ref != NULL)
            {
                return makeRef(ref, PRIO_0 + i);
            }
        }
    }
    for(int i = 0; i < NUM_SEMS; i++)
    {
        if(semaphores[i].init && List_count(semaphores[i].blocked_queue) > 0)
        {
            List_first(semaphores[i].blocked_queue);
            PCB* ref = List_search(semaphores[i].blocked_queue, &COMPARATOR_PCB_PID, &pid);
            if(ref != NULL)
            {
                return makeRef(List_curr(semaphores[i].blocked_queue), SEM_0 + i);
            }
            
        }
    }
    return makeRef(NULL,ERR);
}

/*
    print all info about a referenced PCB
*/

void printPCB(const PCBref proc)
{
    printf("####################\nProcess ID: %i\n", proc.pcb->PID);
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