#include "list.h"

#ifndef TEST_H
#define TEST_H

void resetTestEnv();

void resetPtrList();

int PtrID(struct NODE *ptr);

void printList(struct LIST *list);

void printIntList(struct LIST *list);
#endif
