#include "sim.h"
#include <stdio.h>



/* create new process with specified priority
 - return pid of process, or -1 if fail
*/ 
int Create(int priority)
{
    int pid = genPID();

    PCB* newPCB = malloc(sizeof(PCB));
    newPCB->priority =  priority;
    newPCB->PID = pid;
    memset(newPCB->msg, 0, MSGLENGTH * sizeof(char));
    List_prepend(queues[priority], newPCB);
    return pid;
}



void TotalInfo()
{
    //current process
    printf("[[[[[[[[[[[[[ Current Process ]]]]]]]]]]]]]\n");
    if(current)
    {
        printPCB( (PCBref) {current, RUNNING});
    } else {
        printf("None\n");
    }

    //priority queues
    printf("[[[[[[[[[[[[[ Process Queues ]]]]]]]]]]]]]\n");
    for(int i = 0; i < NUM_QUEUES; i++){
        printf(" ----- Priority %i Queue -----\n", i);
        List_first(queues[i]);
        while(List_curr(queues[i]) != NULL)
        {
            printPCB( (PCBref) {List_curr(queues[i]), i} );
            List_next(queues[i]);
        }
    }

    //send queue
    printf("----- Send Blocked Queue -----\n");
    List_first(sendQueue);
    while(List_curr(sendQueue) != NULL)
    {
        printPCB( (PCBref) {List_curr(sendQueue), SEND} );
        List_next(sendQueue);
    }

    //receive queue
    printf("----- Receive Blocked Queue -----\n");
    List_first(recvQueue);
    while(List_curr(recvQueue) != NULL)
    {
        printPCB( (PCBref) {List_curr(recvQueue), RECV} );
        List_next(recvQueue);
    }

    //semaphores
    printf("[[[[[[[[[[[[[ Semaphores ]]]]]]]]]]]]]\n");
    for(int i = 0; i < NUM_SEMS; i++)
    {
        semaphore sem = semaphores[i];
        printf("----- Semaphore %i -----\n", i);
        if(sem.init != true)
        {
            printf("Not initialized\n");
        } else 
        {
            printf("Counter: %i\n", sem.counter);
            List_first(sem.blocked_queue);
            while(List_curr(sem.blocked_queue) != NULL)
            {
                printPCB( (PCBref) {List_curr(sem.blocked_queue), SEM_0 + i} );
                List_next(sem.blocked_queue);
            }
        }
    }

    //init process
    printf("[[[[[[[[[[[[[ Init Process ]]]]]]]]]]]]]\n");
    printPCB( (PCBref) {&init, INIT});
}