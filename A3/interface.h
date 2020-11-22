#ifndef INTERFACE_H
#define INTERFACE_H
#include "sim.h"


// loop asking user for input, then executing given function
void doCommand();
int getInt();
void getInput(char* buf);

//ask user for parameters of given sim function, then execute it
void doCreate();
void doFork();
void doKill();
void doExit();
void doQuantum();
void doSend();
void doReceive();
void doReply();
void doNewSem();
void doSemP();
void doSemV();
void doProcInfo();
void doTotalInfo();

void Help();
#endif
