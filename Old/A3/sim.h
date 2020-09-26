#ifndef SIM_H
#define SIM_H
#include "list.h"


#define BUFLENGTH 40
#define NUM_QUEUES 4
#define NUM_FLAGS 7
/*

MSG
 - contents
 - target PID

globals
 - init PCB
 - current process - PCB
 - priority queue 0, 1, 2 - list of PCBs
 - blocked queue - list of PCBs
 - message queue - list of hanging messages
 
loop
 - command
*/

enum pcb_state{ PRIO_0, PRIO_1, PRIO_2, BLOCKED, RUNNING, INIT};
enum pcb_flags{ sem_0, sem_1, sem_2,sem_3,sem_4,send_flag,recv_flag};
/*PCB
 - priority
 - message
 - PID
 - semaphore 0, 1, 2, 3, 4 flag, send flag, recieve flag
*/
typedef struct PCB
{
    int priority;
    int PID;
    char msg[BUFLENGTH];
    int flags[7];
} PCB;

/*
 - wraps a reference to a PCB
 - holds state PCB in simulation at time of reference
*/
typedef struct PCBref
{
    PCB* pcb;
    int state;
} PCBref;

/*
 - saves message for target to recieve
*/
typedef struct message
{
    char msg[BUFLENGTH];
    int targetPID;
    int senderPID;
}message;

//******************** User I/O ************************

//fills a buffer with user input
//max length defined by BUFLENGTH
void readline(char* buf);

//get an int from user within range
int getRangeInt(int min, int max);

//get PID from user
int getPID();

//get semaphore ID from user
int getSemID();

//get priority number from user
int getPriority();

//ask user for command char
//perform that command
void doCommand();

//******************* Sim functions ************************

//run at startup, initializes simulation
void initialize();

//increment nextpid, or cycle back to 0 if at max
void incrementpid();

//return a new unique PID
int generatePID();

//check if there exists a process with PID
int validatePID();

//return pointer to process with given pid
PCBref getProcessByPID(int pid);

//kill process by PID
void killProcess(int PID);

//take next process from queues and run it
void runNextProcess();

//requeue process if no longer blocked
void recheckBlock(PCB* pcb);

// genpid
//  - make a unique pid

// loadnext
//  - load next current

// schedulePCB
//  - parameter: ref to PCB
//  - append PCB on correct queue

// killPCB
//  - parameter: ref to target PCB
//  - if kill init, if no others, -1
//                 else end sim
//  - if kill current, load next
//  - kill PCB


//  ****************** commands **************************

// Create
//  - parameters: int priority. return -1 if not 0 <= priority <= 2
//  - if current PCB is init, put new in current
//  - else schedulePCB
//  - return PID
void Create();

// Fork
//  - if current is init, return -1
//  - else copy current, schedulePCB
//  - return PID
void Fork();

// Kill
//  - parameters: int pid
//  - search all lists for PCB with PID and delet
//  - if can't find target, return -1
//  - else return killPCB on target
void Kill();

// Exit 
//  - return killPCB on current
void Exit();

// Quantum
//  - schedulePCB on current
//  - loadnext
void Quantum();

// Send
//  - parameters: target pid, message
//  - check valid pid?
//  - look for target prepped to recieve, deposit msg and unblock
//  - else, place in msg queue
//  - block self on send
void Send();

// Receive
//  - check for first incoming msg in msg queue
//  - print it
//  - if none, block self on receive
void Receive();

// Reply
//  - check for sender with pid
//  - set message, unblock on send
//  - return -1 on fail, 0 on success
void Reply();

// New Semaphore
//  - parameters: int ID, int val
//  - set ID to val
//  - if already set return -1, else return 0 
void NewSem();

// Semaphore P
//  - parameters: int ID
//  - decrement ID
//  - if ID < 0, block current on ID
void SemP();

// Semaphore V
//  - parmeters: int ID
//  - increment ID
//  - unblock first PID blocked on ID
void SemV();

// Procinfo
//  - parameters: int pid
//  - dump state of current to screen
void ProcInfo();

// Totalinfo
//  - display all process queues and contents
void TotalInfo();


#endif