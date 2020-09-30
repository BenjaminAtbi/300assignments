
#include "list.h"
#include <stdlib.h>

Node nodes[LIST_MAX_NUM_NODES];
List heads[LIST_MAX_NUM_HEADS];
Node headNodes[LIST_MAX_NUM_HEADS];

//list of free nodes
//nodes point to themself as item
List free_nodes = { NULL, NULL, NULL, 0};
List free_heads = { NULL, NULL, NULL, 0};


//*********************************//
//********** PROTOTYPES ***********//
//*********************************//
void Init();
static void SetList(List* list, Node* first, Node* last, Node* current);
static void ClearList(List* list);
static void ClearNode(Node* node);
static void JoinEdge(Node* left, Node* right);
static void JoinDoubleEdge(Node* left, Node* node, Node* right);
// static void PlaceNodeLast(List* list, Node* node);
static void AddFirstItem(List* list, void *item);
static void* GetItem(List* list);

//*********************************//
//*******  Util Functions  ********//
//*********************************//

//place unallocated heads and nodes into lists
void Init()
{
    //string heads
    SetList(&free_heads, &headNodes[0], &headNodes[LIST_MAX_NUM_HEADS], &headNodes[0], LIST_MAX_NUM_HEADS);
    JoinEdge(NULL, &headNodes[0]);
    headNodes[0].item = &heads[0];
    JoinEdge(&headNodes[LIST_MAX_NUM_HEADS], NULL);
    for(int i = 1; i < LIST_MAX_NUM_HEADS; i++)
    {
        JoinEdge(&headNodes[i-1], &headNodes[i]);
        headNodes[i].item = &heads[i];
    }

    //string nodes
    SetList(&free_nodes, &nodes[0], &nodes[LIST_MAX_NUM_NODES], &nodes[0], LIST_MAX_NUM_NODES);
    JoinEdge(NULL, &nodes[0]);
    nodes[0].item = &nodes[0];
    JoinEdge(&nodes[LIST_MAX_NUM_NODES], NULL);
    for(int i = 1; i < LIST_MAX_NUM_NODES; i++)
    {
        JoinEdge(&nodes[i-1], &nodes[i]);
        nodes[i].item = &nodes[i];
    }
}

//set ptr values for a list
static void SetList(List* list, Node* first, Node* last, Node* current)
{
    list->first = first;
    list->last = last;
    list->current = current;
}

// resets all list values
static void ClearList(List* list)
{
    list->first = NULL;
    list->last = NULL;
    list->current = NULL;
    list->length = 0;
}

// resets all node values
static void ClearNode(Node* node)
{
    node->item = NULL;
    node->next = NULL;
    node->prev = NULL;
}

// connect two nodes by shared edge
static void JoinEdge(Node* left, Node* right)
{
    if(left) left->next = right;
    if(right) right->prev = left;
}

// connect a node with two others
static void JoinDoubleEdge(Node* left, Node* node, Node* right)
{
    JoinEdge(left, node);
    JoinEdge(node, right);
}

//place a new node in a list
// * list: list to place element in
// * ref: node in list to place node becide
// * node: node to be placed
// * relation: which side of the reference node to place new node
// static void PlaceNodeLast(List* list, Node* node)
// {

// }

//add an item to a list with 0 elements
//assumes list has 0 elements
static void AddFirstNode(List* list, Node* node)
{
    JoinDoubleEdge(NULL, node, NULL);
    list->first = node;
    list->last = node;
    list->length = 1;
}

//gets a node from the list of free nodes 
//returns NULL on a fail
static Node* GetFreeNode()
{
    if(free_nodes.length == 0)
    {
        return NULL;
    } else if(free_nodes.length == 1)
    {
        Node* node = free_nodes.last;
        SetList(&free_nodes, NULL, NULL, NULL);
        free_nodes.length = 0;
        ClearNode(node);
        return node;
    } else
    {   
        Node* node = free_nodes.last;
        free_nodes.last = free_nodes.last->prev;
        free_nodes.length--;
        ClearNode(node);
        return node;
    }
}

//return an unused node to free_nodes
static void ReturnNode(Node* node)
{
    if(free_nodes.length == 0)
    {
        AddFirstNode(&free_nodes, node);
    } else {
        JoinDoubleEdge(free_nodes.last, node, NULL);
        free_nodes.last = node;
        free_nodes.length++;
    }
}

//*********************************//
//********  Core Functions ********//
//*********************************//


List* List_create()
{
    //get list head from free_lists
    //clear head
    //if none available, return null 
}

int List_count(List* pList);

void* List_first(List* pList);

void* List_last(List* pList); 

void* List_next(List* pList);

void* List_prev(List* pList);

void* List_curr(List* pList);

int List_add(List* pList, void* pItem);

int List_insert(List* pList, void* pItem);

int List_append(List* pList, void* pItem)
{
    //if length 0, addFirstItem
    //if length >0, 
    //  place after last
    //  move last forward
}

int List_prepend(List* pList, void* pItem);

void* List_remove(List* pList);

void List_concat(List* pList1, List* pList2);

typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn);

void* List_trim(List* pList);

typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg);