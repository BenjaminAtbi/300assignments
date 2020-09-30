#include "list.h"

#ifndef _TEST_H_
#define _TEST_H_

//nodes and heads to be allocated 
extern Node nodes[LIST_MAX_NUM_NODES];
extern List heads[LIST_MAX_NUM_HEADS];
extern List free_nodes;
extern List free_heads;

void Init();

void listErr(List* list, char* err);

void generalErr(char* err);

void printList(List* list);

void printHeadDetails(List* list);

void printNodeDetails(Node* node);

void printNodeSignature(Node* node);

int GetNodeID(Node* node);

void ValidateList(List* list);

#endif