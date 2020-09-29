
#include "list.h"
#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/***********************
TEMPORARY
*************/ 

Node nodes[LIST_MAX_NUM_NODES];
List heads[LIST_MAX_NUM_HEADS];
List free_nodes = { 0, NULL, NULL, NULL};
List free_heads = { 0, NULL, NULL, NULL};

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
    printf(" length: %i ]]\n", list->len);
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
    generalErr("GetNodeID: node not in list");
}

//
void ValidateList(List* list)
{
    if(list->len == 0 && (list->first != NULL) || (list->last != NULL))
    {
        listErr(list, "ValidateList: list of length 0 has nodes\n");
    }
    if(list->len != 0 && (list->first == NULL) || (list->last == NULL))
    {
        listErr(list, "ValidateList: list of length >0 has NULL pointer\n");
    }
}

//initial self-sufficient test, string free nodes head with node 3 and 4
void Test0(){
    int e1 = 33;
    int e2 = 44;
    free_nodes.len = 2;
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

int main()
{
    Test0();
}

