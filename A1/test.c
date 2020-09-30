
#include "list.h"
#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/***********************
TEMPORARY DECLARATION
*************/ 

// Node nodes[LIST_MAX_NUM_NODES];
// List heads[LIST_MAX_NUM_HEADS];
// List free_nodes = { 0, NULL, NULL, NULL};
// List free_heads = { 0, NULL, NULL, NULL};


/***********************
TEMPORARY DECLARATION
*************/ 

//exit program upon error in list
void listErr(List* list, char* err)
{
    printf("%s",err);
    printList(list);
    exit(1);
}

//exit with non-specific error
void generalErr(char* err)
{
    printf("%s",err);
    exit(1);
}


void printList(List* list)
{
    printHeadDetails(list);
    printf("### First ###\n");
    printNodeDetails(list->first);
    printf("### Last ###\n");
    printNodeDetails(list->last);
    printf("### Current ###\n");
    printNodeDetails(list->current);
    printf("### Traversal ###\n");
    Node* node = list->first;
    for(int i = 0; node && i <= LIST_MAX_NUM_NODES; i++)
    {
        if(i == LIST_MAX_NUM_NODES)
        {
            printf("TRAVERSAL MAXED OUT\n");
            break;
        }
        printNodeDetails(node);
        node = node->next;
    }
}

void printHeadDetails(List* list)
{
    printf("[[ First: ");
    printNodeSignature(list->first);
    printf(" Last: ");
    printNodeSignature(list->last);
    printf(" Current: ");
    printNodeSignature(list->current);
    printf(" length: %i ]]\n", list->length);
}

//print all details regarding node
void printNodeDetails(Node* node)
{
    printf("[ ");
    printNodeSignature(node->prev);
    printf(" <- ");
    printNodeSignature(node);
    if (node->item) printf(" { %i }", *(int*)node->item);
    else printf(" { NULL }");
    printf(" -> ");
    printNodeSignature(node->next);
    printf(" ]\n");
}

//print minimal representation of node
void printNodeSignature(Node* node)
{
    
    if(node) printf("( ID: %i )",GetNodeID(node));
    else printf("( NULL )");
}

//return ID of a node
// * ID is index in node array
int GetNodeID(Node* node)
{
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++)
    {
        if(&nodes[i] == node){
            return i;
        }
    }
    return -1;
}

//
void ValidateList(List* list, Void* control_list)
{
    if(list->length == 0 && (list->first != NULL) || (list->last != NULL))
    {
        listErr(list, "ValidateList: list of length 0 has nodes\n");
    }
    if(list->length != 0 && (list->first == NULL) || (list->last == NULL))
    {
        listErr(list, "ValidateList: list of length >0 has NULL pointer\n");
    }
    if(list->first->prev != NULL)
    {
        listErr(list, "ValidateList: first node not preceded by NULL\n");
    }
    Node* node = list->first;
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++)
    {
        if(node->item != control_list[i])
        {
            listErr(list, "element incorrect\n");
        }
        if(node->next == NULL)
        {
            if(list->length != i){
                listErr(list, "length not correct\n");
            }
            break;
        }
    }
    if(list->last != node)
    {
        listErr(list, "last element in traversal != last pointer");
    }

//initial self-sufficient test, string free nodes head with node 3 and 4
void test0(){
    int e1 = 33;
    int e2 = 44;
    free_nodes.length = 2;
    free_nodes.first = &nodes[3];
    free_nodes.last = &nodes[4];
    free_nodes.current = &nodes[4];
    nodes[3].prev = NULL;
    nodes[3].next = &nodes[4];
    nodes[3].item = &e1;
    nodes[4].prev = &nodes[3];
    nodes[4].next = NULL;
    nodes[4].item = &e2;
    printList(&free_nodes);
}

//test Init
void test1(){
    Init();
    printList(&free_nodes);
    printList(&free_heads);
}

//test
int main()
{
    test1();
}

