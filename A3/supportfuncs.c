#include "sim.h"

/*
    initialize simulation
     - create init process
     - create process queues
     - create message queue
*/
void Initialize()
{
    init = (PCB) {.priority = -1, .PID =  0};
    memset(init.msg, 0, MSGLENGTH * sizeof(char));
    memset(init.flags, 0, NUM_FLAGS * sizeof(int));
    current = &init;

    for(int i = 0; i < NUM_QUEUES; i++){
        queues[i] = List_create();
    }

    messages = List_create();
}

/*
    set message field of a process
*/
void set_message(PCB* process, char* msg)
{
    memcpy(process->msg, msg, MSGLENGTH * sizeof(char));
}