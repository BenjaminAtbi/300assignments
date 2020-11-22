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
    enqueueProcess(queues[current->priority], current);
    current = NULL;
    updateSim();
}

void Send(int pid, char* msg)
{
    PCBref ref = getPCBbyPID(pid);
    //error finding target
    if (ref.state == ERR)
    {
        printf("cannot find PCB with that PID\n");
        return;
    //target is self
    } else if(ref.pcb == current)
    {
        printf("process can't send message to itself\n");
        return;
    //target is waiting for message
    } else if(ref.state == RECV)
    {
        List_first(recvQueue);
        List_search(recvQueue, &COMPARATOR_PCB_PID, &pid);
        PCB* receiver = List_remove(recvQueue);
        setMessage(receiver, msg);
        enqueueProcess(queues[receiver->priority], receiver);
        printf("delivered message to process (PID %i)\n",pid);
    //target not waiting for message
    } else {
        message* new_message = makeMessage(pid, current->PID, msg);
        List_prepend(messages, new_message);
        printf("queued message for process (PID %i)\n",pid);
    }
    if(current != init){
        printf("blocking sender (PID %i) until reply\n",current->PID);
        enqueueProcess(sendQueue, current);
        current = NULL;
        updateSim();
    }
}

void Receive()
{
    //if can find a message waiting, receive it
    List_first(messages);
    List_search(messages, &COMPARATOR_MSG_RECVR, &current->PID);
    if(List_curr(messages) != NULL)
    {
        message* incoming = List_remove(messages);
        setMessage(current, incoming->msg);
        printf("received message from process (PID %i):\n",incoming->sender_PID);
        printf("%s\n",current->msg);
        free(incoming);
    } else {
        if(current != init){
            printf("blocking until message received\n");
            enqueueProcess(recvQueue, current);
            current = NULL;
            updateSim();
        }

    }
}

void Reply()
{
    
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