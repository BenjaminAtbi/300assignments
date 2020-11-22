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

    updateSim();
    return pid;
}

/* copy currently running process
 - return pid of new process
 */
int Fork()
{
    if(current == init)
    {
        return 0;
    } else if(current == NULL)
    {
        return -1;
    }

    int pid = genPID();
    PCB* newPCB = malloc(sizeof(PCB));
    newPCB->priority =  current->priority;
    newPCB->PID = pid;
    memcpy(newPCB->msg, current->msg, MSGLENGTH * sizeof(char));
    List_prepend(queues[newPCB->priority], newPCB);

    updateSim();
    return pid;
}

/* kill process identified by PID
 - cannot kill init unless no other processes exist
*/
void Kill(int pid)
{
    PCBref ref = getPCBbyPID(pid);
    if (ref.state == ERR)
    {
        printf("cannot find PCB with that PID\n");
        return;
    }
    killProcess(ref);
}

/* kill currently running process
 - cannot kill init unless no other processes exist
 */
void Exit()
{
    killProcess(makeRef(current, RUNNING));
}


/* expire the current processes time quantum,
    - put on end of queue and recalculate current program;
*/

void Quantum()
{
    if(current->priority >= 0 && current->priority < NUM_QUEUES)
    {
        List_prepend(queues[current->priority], current);
        current = NULL;
    }
    updateSim();
}


/* dump info on a single process
*/
void ProcInfo(int pid)
{
    PCBref ref = getPCBbyPID(pid);
    if (ref.state == ERR)
    {
        printf("cannot find PCB with that PID\n");
        return;
    }
    printPCB(ref);
}

/* Dump info on all aspects of simulation
*/
void TotalInfo()
{
    //current process
    printf("[[[[[[[[[[[[[ Current Process ]]]]]]]]]]]]]\n");
    if(current->PID == 0)
    {
        printPCB( makeRef(current, INIT));
    } else if (current == NULL)
    {
        printf("None\n");
    } else
    {
        printPCB( makeRef(current, RUNNING));
    }

    //priority queues
    printf("[[[[[[[[[[[[[ Process Queues ]]]]]]]]]]]]]]\n");
    for(int i = 0; i < NUM_QUEUES; i++){
        printf("----- Priority %i Queue -----\n", i);
        List_first(queues[i]);
        while(List_curr(queues[i]) != NULL)
        {
            printPCB( makeRef(List_curr(queues[i]), i) );
            List_next(queues[i]);
        }
    }

    //send queue
    printf("----- Send Blocked Queue -----\n");
    List_first(sendQueue);
    while(List_curr(sendQueue) != NULL)
    {
        printPCB( makeRef(List_curr(sendQueue), SEND) );
        List_next(sendQueue);
    }

    //receive queue
    printf("----- Receive Blocked Queue -----\n");
    List_first(recvQueue);
    while(List_curr(recvQueue) != NULL)
    {
        printPCB( makeRef(List_curr(recvQueue), RECV) );
        List_next(recvQueue);
    }

    //semaphores
    printf("[[[[[[[[[[[[[[[ Semaphores ]]]]]]]]]]]]]]]]\n");
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
                printPCB( makeRef(List_curr(sem.blocked_queue), SEM_0 + i) );
                List_next(sem.blocked_queue);
            }
        }
    }

    //init process
    printf("[[[[[[[[[[[[[[ Init Process ]]]]]]]]]]]]]]]\n");
    printPCB( makeRef(init, INIT));
}