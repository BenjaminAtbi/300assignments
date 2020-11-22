#include "sim.h"
#include "interface.h"


//request input until input is a single, valid char, then perform specified command
void doCommand()
{
    printf("input next command\n");
    bool valid = false;
    while(!valid)
    {
        printf("> ");
        char c = getchar();
        bool single = true;
        while(getchar() != '\n')
        {
            single = false;
        }

        if(single){
                switch(c)
            {
                case 'h':
                case 'H': Help(); valid = 1; break;
                case 'c':
                case 'C': doCreate(); valid = 1; break;
                case 'f':
                case 'F': doFork(); valid = 1; break;
                case 'k':
                case 'K': doKill(); valid = 1; break;
                case 'e':
                case 'E': doExit(); valid = 1; break;
                case 'q': 
                case 'Q': doQuantum(); valid = 1; break;
                case 's': 
                case 'S': doSend(); valid = 1; break;
                case 'r': 
                case 'R': doReceive(); valid = 1; break;
                case 'y':
                case 'Y': doReply(); valid = 1; break;
                case 'n':
                case 'N': doNewSem(); valid = 1; break;
                case 'p':
                case 'P': doSemP(); valid = 1; break;
                case 'v':
                case 'V': doSemV(); valid = 1; break;
                case 'i':
                case 'I': doProcInfo(); valid = 1; break;
                case 't':
                case 'T': doTotalInfo(); valid = 1; break;
                default: printf(" \'%c\' is an invalid command\n",c);
            }
        } else {
            printf("invalid command\n");
        }
    }
}

void Help()
{
    printf("Help      (h/H): list commands\n");
    printf("Create    (c/C): create new process\n");
    printf("Fork      (f/F): copy currently running process\n");
    printf("Kill      (k/K): kill a target process by PID\n");
    printf("Exit      (e/E): kill the currently running process\n");
    printf("Quantum   (q/Q): expire the time quantum of the currently running process\n");
    printf("Send      (s/S): send message to another process - block until reply\n");
    printf("receive   (r/R): receive a message - block until one arrives\n");
    printf("Reply     (y/Y): unblock a sender and deliver reply\n");
    printf("NewSem    (n/N): initialize a semaphore\n");
    printf("SemP      (p/P): execute semaphore P operation for currently running process\n");
    printf("SemV      (v/V): execute semaphore V operation for currently running process\n");
    printf("ProcInfo  (i/I): display complete state of process by PID\n");
    printf("TotalInfo (t/T): display complete state of simulation\n");
}


//get int from user
int getInt()
{
    printf("> ");
    int i;
    while(scanf(" %i", &i) != 1)
    {
        printf("not an integer\n");
        while(getchar() != '\n');
        printf("> ");
    }
    while(getchar() != '\n');
    return i;
}

/* 
    do functions
*/

void doCreate()
{
    printf("Input a priority for the new process (0 - 2)\n");
    int prio = getInt();
    while(prio > 2 || prio < 0)
    {
        printf("priority not within valid range (0 - 2)\n");
        prio = getInt();
    }
    int pid = Create(prio);
    if(pid != -1)
    {
        printf("successfully create process with priority %i (PID: %i)\n", prio, pid);
    } else
    {
        printf("error creating process\n");
    }
    
}

void doFork()
{
    int val = Fork();
    if(val == 0)
    {
        printf("Can't fork init process\n");
    } else if(val == -1)
    {
        printf("no active process to fork\n");
    } else
    {
        printf("Forked current process. New process has PID %i\n", val);
    }
}

void doKill(){
    printf("Input the PID of the process you want to kill\n");
    Kill(getInt());
}

void doExit(){
    Exit();
}

void doQuantum(){
    printf("Time quantum expired\n");
    Quantum();
}

void doSend(){}
void doReceive(){}
void doReply(){}
void doNewSem(){}
void doSemP(){}
void doSemV(){}

void doProcInfo(){
    printf("Input process ID\n");
    ProcInfo(getInt());
}

void doTotalInfo(){
    TotalInfo();
}

int main()
{
    printf("###########################\n Initializing Simulation\n###########################\n");
    printf("Available commands: \n");
    Initialize();
    Help();
    while(true) 
    {
        doCommand();
    }
}