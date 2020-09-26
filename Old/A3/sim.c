#include "sim.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h> 

PCB init = {.priority = -1, .PID = 0};

PCB* current;


// 0 - 2 are priority queues. 3 is Blocked queue
LIST* queues[NUM_QUEUES];
LIST* messages;
// priority counters. prevent starvation of lower queues
int prio_counter[3] = {2,2,1};

//char buffer for user input
char* buf;

//counter for new pid's
int lastpid = 0;

int semaphore[5] = {0,0,0,0,0};
int seminit[5] = {0,0,0,0,0};

//*********************** Helper Functions ********************************

void initialize(){
    
    buf = (char*)malloc(BUFLENGTH * sizeof(char));
    
    for(int i = 0; i < NUM_QUEUES; i++){
        queues[i] = ListCreate();
    }
    messages = ListCreate();
    memset(init.msg, 0, BUFLENGTH * sizeof(char));
    memset(init.flags, 0, NUM_FLAGS * sizeof(int));
    current = &init;
}

//reads a single line of input data from terminal
//fill a char buffer terminated by \n
void readline(char *buf)
{
    memset(buf, 0, BUFLENGTH * sizeof(char));
    int i = 0;
    char c = '0';
    while(c != '\n')
    {
        c = getchar();
        buf[i] = c;

        if(i < BUFLENGTH-2)
        {
            i++;   
        } else {
            buf[BUFLENGTH-1] = '\n';
            break;
        }
    }
    fflush(stdin);
}

int generatePID()
{
    lastpid++;
    return lastpid;
}

int validatePID(int pid)
{
    PCBref ref = getProcessByPID(pid);
    return ref.pcb != NULL; 
}

void printline(char *buf)
{
    for (int i = 0; i < BUFLENGTH; i++){
        printf("%c",buf[i]);
    }
}

//get a valid PID from user
int getPID()
{
    printf("input a PID\n");

    while (1){
        readline(buf);

        //get leading numerics
        int len = 0;
        for (int i = 0; buf[i] >= 48 && buf[i] <= 57 && i < BUFLENGTH; i++){
            len++;
        }
        if(len <=0){
            printf("invalid input\n");
            continue;
        }
        //convert to int
        char substr[len];
        memcpy(substr, &buf[0], len);
        int pid = atoi(substr);

        if(validatePID(pid)){
            return pid;
        } else {
            printf("invalid input\n");
        }
    }
}

//get an int from user within range
int getRangeInt(int min, int max)
{
    while (1){
        readline(buf);

        //get leading numerics
        int len = 0;
        for (int i = 0; buf[i] >= 48 && buf[i] <= 57 && i < BUFLENGTH; i++){
            if(buf[i] >= 48 && buf[i] <= 57){
                len++;
            }
        }
        if(len <=0){
            printf("invalid input\n");
            continue;
        }

        //convert to int
        char substr[len];
        memcpy(substr, &buf[0], len);
        int num = atoi(substr);

        if(num >= min && num <= max){
            return num;
        } else {
            printf("invalid input\n");
            continue;
        }
    }
}

void killProcess(int PID)
{
    PCBref ref_pcb = getProcessByPID(PID);
    PCB* target = ref_pcb.pcb;
    
    //init special case
    if(target->PID == 0){
        if(ListCount(queues[0]) == 0 && ListCount(queues[1]) == 0 && ListCount(queues[2]) == 0 && ListCount(queues[3]) == 0)
        {
            printf("exiting simulation\n");
            exit(0);
        } else {
            printf("cannot kill init process\n");
            return;
        }
    }

    if(target == current){
        printf("killed running process (PID: %i)\n", target->PID);
        free(target);
        runNextProcess();
        return;
    }

    for(int i = 0; i < NUM_QUEUES; i++)
    {
        PCB* queue_pcb = ListFirst(queues[i]);
        while(queue_pcb != NULL && queue_pcb != target)
        {
            queue_pcb = ListNext(queues[i]);
        }
        if(target == queue_pcb){
            ListRemove(queues[i]);
            printf("killed process (PID: %i)\n", target->PID);
            free(target);
            return;
        }
    }
    printf("kill failed\n");
}

//get an priority int from user
int getPriority()
{
    printf("input process priority\n");
    return getRangeInt(0,2);
}

void blockProcess(PCB* process)
{
    if(process->PID == 0)
    {
        printf("blocking init failed\n");
    } else {
        ListAppend(queues[BLOCKED], process);
    }
}


//get a semaphore ID from user
int getSemID()
{
    printf("input semaphore ID\n");
    return getRangeInt(0,4);
}

void recheckBlock(PCB* pcb)
{
    int state = 0;
    for(int i = 0; i < 7; i++){
        if(pcb->flags[i] == 1){
            state = 1;
        }
    }
    if(state == 0){
        PCB* queue_pcb = ListFirst(queues[BLOCKED]);
        while(queue_pcb != NULL && queue_pcb != pcb)
        {
            queue_pcb = ListNext(queues[BLOCKED]);
        }
        if(pcb == queue_pcb){
            ListRemove(queues[BLOCKED]);
            ListAppend(queues[pcb->priority], pcb);
            printf("requeued process: %i\n", pcb->PID);
        } else {
            printf("recheck failed\n");
        }
    }
}


//comparator for getProcessByPID
int comparePID(PCB* item, int* val)
{
    return item->PID == *val;
}



//return pointer to PCB with given pid
PCBref getProcessByPID(int pid)
{
    if(pid == 0)
    {
        return (PCBref){ .pcb = &init, .state = INIT};
    }

    if(current->PID == pid)
    {
        return (PCBref){ .pcb = current, .state = RUNNING};
    }

    PCB* result;
    for(int i = 0; i < 4; i++){
        ListFirst(queues[i]);
        result = ListSearch(queues[i],comparePID,&pid);
        if (result != NULL)
        {
            return (PCBref){ .pcb = result, .state = i};
        }
    }
    return (PCBref){ .pcb = NULL, .state = -1};
}

//tick down counters for higher priority queues, resetting and passing to next queue if counter = 0 
//each queue gets ~ 2x as much runtime as next queue
void runNextProcess()
{
    //do priority 0 if one exists
    //do if counter > 0, or there are none avalable in queue 1 or 2
    if(ListCount(queues[0]) > 0)
    {
        if(prio_counter[0] > 0){
            prio_counter[0]--;
            ListFirst(queues[0]);
            current = ListRemove(queues[0]);
            printf("running process: %i\n",current->PID);
            return;
        } else if (prio_counter[0] == 0 && ListCount(queues[1]) == 0 && ListCount(queues[2]) == 0)
        {
            ListFirst(queues[0]);
            current = ListRemove(queues[0]);
            printf("running process: %i\n",current->PID);
            return;
        } 
    }
    //do priority 1 if one exists
    //do if counter > 0, or there are none available in queue 2
    //reset counter for priority 0
    if(ListCount(queues[1]) > 0)
    {
        if(prio_counter[1] > 0){
            prio_counter[1]--;
            prio_counter[0] = 2;
            ListFirst(queues[1]);
            current = ListRemove(queues[1]);
            printf("running process: %i\n",current->PID);
            return;
        } else if (prio_counter[1] == 0 && ListCount(queues[2]) == 0)
        {
            prio_counter[0] = 2;
            ListFirst(queues[1]);
            current = ListRemove(queues[1]);
            printf("running process: %i\n",current->PID);
            return;
        } 
    }

    //do priority 2 if one exists
    //reset counter for priority 0 and 1
    if(ListCount(queues[2]) > 0)
    {
        prio_counter[0] = 2;
        prio_counter[1] = 2;
        ListFirst(queues[2]);
        current = ListRemove(queues[2]);
        printf("running process: %i\n",current->PID);
        return;
    } 

    //run init
    current = &init;
    printf("running process: %i\n",current->PID);
}

void doCommand()
{
    printf("input next command\n");
    int valid = 0;
    while (!valid){

        readline(buf);
        switch(buf[0])
        {
            case 'c':
            case 'C': Create(); valid = 1; break;
            case 'f':
            case 'F': Fork(); valid = 1; break;
            case 'k':
            case 'K': Kill(); valid = 1; break;
            case 'e':
            case 'E': Exit(); valid = 1; break;
            case 'q': 
            case 'Q': Quantum(); valid = 1; break;
            case 's': 
            case 'S': Send(); valid = 1; break;
            case 'r': 
            case 'R': Receive(); valid = 1; break;
            case 'y':
            case 'Y': Reply(); valid = 1; break;
            case 'n':
            case 'N': NewSem(); valid = 1; break;
            case 'p':
            case 'P': SemP(); valid = 1; break;
            case 'v':
            case 'V': SemV(); valid = 1; break;
            case 'i':
            case 'I': ProcInfo(); valid = 1; break;
            case 't':
            case 'T': TotalInfo(); valid = 1; break;
            default: printf("invalid command\n");
        }
    }
}

//************************** COMMANDS ****************************

void Create()
{
    int pid = generatePID();
    int prio = getPriority();
    
    PCB* newPCB = malloc(sizeof(PCB));
    newPCB->priority = prio;
    newPCB->PID = pid;
    memset(newPCB->msg, 0, BUFLENGTH * sizeof(char));
    memset(newPCB->flags, 0, NUM_FLAGS * sizeof(int));
    
    ListAppend(queues[prio], newPCB);
    printf("created process with PID %i\n", pid);
}

void Fork()
{
    PCB* newPCB = malloc(sizeof(PCB));
    *newPCB = *current;
    int pid = generatePID();
    newPCB->PID = pid;
    ListAppend(queues[newPCB->priority], newPCB);
    printf("created process with PID %i\n", pid);
}

void Kill(){
    killProcess(getPID());
}

void Exit(){
    killProcess(current->PID);
}

//move process into execution following scheduling scheme
//run next process
void Quantum(){
    if(current->PID != 0)
    {
        ListAppend(queues[current->priority], current);
    }
    runNextProcess();
}

void Send(){
    PCBref target = getProcessByPID(getPID());
    printf("input message\n");
    readline(buf);
    if(target.pcb->flags[recv_flag] == 1){
        target.pcb->flags[recv_flag] = 0;
        memcpy(target.pcb->msg, buf, BUFLENGTH * sizeof(char));
        recheckBlock(target.pcb);
        printf("message recieved\n");
    } else {
        message* package = malloc(sizeof(message));
        memcpy(package->msg, buf, BUFLENGTH * sizeof(char));
        package->targetPID = target.pcb->PID;
        package->senderPID = current->PID;
        ListAppend(messages, package);
        printf("message saved for next recieve\n");
    }
    current->flags[send_flag] = 1;
    blockProcess(current);
    runNextProcess();
}

void Receive(){
    message* package = ListFirst(messages);
    while(package != NULL && package->targetPID != current->PID)
    {
        package = ListNext(messages);
    }
    if(package != NULL){
        ListRemove(messages);
        printf("recieved saved message from process %i: ",package->senderPID);
        printline(package->msg);
        printf("\n");
        free(package);
    } else {
        current->flags[recv_flag] = 1;
        blockProcess(current);
        runNextProcess();
    }
}

void Reply()
{
    PCBref target = getProcessByPID(getPID());
    printf("input message\n");
    readline(buf);
    if(target.pcb->flags[send_flag] == 1){
        target.pcb->flags[send_flag] = 0;
        memcpy(target.pcb->msg, buf, BUFLENGTH * sizeof(char));
        recheckBlock(target.pcb);
        printf("reply recieved\n");
    } else {
        printf("target not open for reply\n");
    }
}



void NewSem(){
    int semID = getSemID();
    if(seminit[semID] == 1)
    {
        printf("Semaphore already initialized\n");
    } else 
    {
        printf("enter semaphore starting value\n");
        semaphore[semID] = getRangeInt(0,INT_MAX);
        seminit[semID] = 1;
    }
}

void SemP(){
    int semID = getSemID();
    if(seminit[semID] == 0)
    {
        printf("Semaphore not initialized\n");
    } else 
    {   
        semaphore[semID]--;
        if (semaphore[semID] < 0)
        {
            current->flags[semID] = 1;
            blockProcess(current);
            runNextProcess();
        } 
    }
}

void SemV(){
    int semID = getSemID();
    if(seminit[semID] == 0)
    {
        printf("Semaphore not initialized\n");
    } else 
    {   
        semaphore[semID]++;
        if (semaphore[semID] - 1  < 0)
        {
            PCB* blocked_pcb = ListFirst(queues[BLOCKED]);
            while(blocked_pcb != NULL && blocked_pcb->flags[semID] == 0)
            {
                blocked_pcb = ListNext(queues[BLOCKED]);
            }
            if(blocked_pcb == NULL){
                printf("error finding blocked process\n");
            } else {
                ListRemove(queues[BLOCKED]);
                ListAppend(queues[blocked_pcb->priority], blocked_pcb);
                printf("unblocked process %i \n", blocked_pcb->PID);
            } 
        } 
    }
}


//print info about a process
void ProcInfo()
{
    PCBref processref = getProcessByPID(getPID());
    PCB *process = processref.pcb;

    printf("Process ID: %i\n", process->PID);
    printf("Process state: ");
    switch(processref.state)
    {
        case 0: printf("priority 0 queue\n");break;
        case 1: printf("priority 1 queue\n");break;
        case 2: printf("priority 2 queue\n");break;
        case 3: printf("Blocked queue\n");break;
        case 4: printf("Running\n");break;
        case 5: printf("is Init function\n");
    }
    printf("message: ");
    for (int i = 0; i < BUFLENGTH && process->msg[i] != 0; i++)
    {
        printf("%c", process->msg[i]);
    }
    printf("\nflags: ");
    for (int i = 0; i < 5; i++)
    {
        printf("sem%i: %i, ", i, process->flags[i]);
    }
    printf("write: %i, read: %i\n", process->flags[5], process->flags[6]);
}

//print info about simulation state
void TotalInfo(){

    printf("Running: [ pid %i ]\n", current->PID);
    for(int i = 0; i < NUM_QUEUES; i++)
    {
        if(i == 3){
            printf("blocked: ");
        } else {
            printf("priority %i: ",i);
        }
        PCB* pcb = ListFirst(queues[i]);
        while(pcb != NULL)
        {
            printf(" [ pid %i ] ", pcb->PID);
            pcb = ListNext(queues[i]);
        }
        printf("\n");
    }
    printf("semaphores: ");
    for(int i = 0; i < 5; i++)
    {
        printf(" | sem %i: %i ",i, semaphore[i]);
    }
    printf("|\n");
}

int main(){
    initialize();

    while(1)
    {
        doCommand();
    }
}