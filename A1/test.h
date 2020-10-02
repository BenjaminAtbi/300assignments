#include "list.h"

#ifndef _TEST_H_
#define _TEST_H_

//nodes and heads to be allocated 
// extern Node nodes[LIST_MAX_NUM_NODES];
// extern List heads[LIST_MAX_NUM_HEADS];
// extern Node headNodes[LIST_MAX_NUM_HEADS];
// extern List free_nodes;
// extern List free_heads;

//exit program upon error in list
void listErr(List* list, char* err);

//exit with non-specific error
void generalErr(char* err);

//prints details regarding list
void printList(List* list);

//prints all details regarding head
void printHeadDetails(List* list);

//print all details regarding node
void printNodeDetails(Node* node);

//print minimal representation of node
void printNodeSignature(Node* node);

//return ID of a node
// * ID is index in node array
int GetNodeID(Node* node);

//set first three values of a control list
void setControlList(void** control_list, void* first_item, void* last_item, void* current_item);

//validate that all aspects of a list are correct
//control list is list of items that list should contain.
//control_list[0-2] are list ptrs first, last, current.
//control_list[3-n] are correct contents of list nodes, in order.
void validateList(List* list, void** control_list);


#endif