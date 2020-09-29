#include "list.h"

#ifndef _TEST_H_
#define _TEST_H_


void listErr(List* list, char* err);

void generalErr(char* err);

void printList(List* list);

void printHeadDetails(List* list);

void printNodeDetails(Node* node);

void printNodeSignature(Node* node);

int GetNodeID(Node* node);

void ValidateList(List* list);

#endif