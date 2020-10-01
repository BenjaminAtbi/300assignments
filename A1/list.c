
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

Node nodes[LIST_MAX_NUM_NODES];
List heads[LIST_MAX_NUM_HEADS];
Node headNodes[LIST_MAX_NUM_HEADS];

//list of free nodes
//nodes point to themself as item
List free_nodes = { NULL, NULL, NULL, 0};
List free_heads = { NULL, NULL, NULL, 0};

//flag for first initialization
static int INIT = 0;

//*********************************//
//********** PROTOTYPES ***********//
//*********************************//
static void Init();
static void SetList(List* list, Node* first, Node* last, Node* current);
static void SetListOOB(List* list, Node* first, Node* last, Node* current, enum ListOutOfBounds outofbounds);
static void ClearList(List* list);
static void ClearNode(Node* node);
static void JoinEdge(Node* left, Node* right);
static void JoinDoubleEdge(Node* left, Node* node, Node* right);
static void AddFirstNode(List* list, Node* node);
static void* GetItem(List* list);
static Node* GetFreeNode();
static void ReturnNode(Node* node);
static List* GetFreeHead();
static void ReturnHead(List* head);

//*********************************//
//*******  Util Functions  ********//
//*********************************//

//place unallocated heads and nodes into lists
static void Init()
{
    //string heads
    //head list functions as stack, with current pointing to top element
    SetList(&free_heads, &headNodes[0], &headNodes[LIST_MAX_NUM_HEADS-1], &headNodes[LIST_MAX_NUM_HEADS-1]);
    free_heads.length = LIST_MAX_NUM_HEADS;
    JoinEdge(NULL, &headNodes[0]);
    headNodes[0].item = &heads[0];
    JoinEdge(&headNodes[LIST_MAX_NUM_HEADS-1], NULL);
    for(int i = 1; i < LIST_MAX_NUM_HEADS; i++)
    {
        JoinEdge(&headNodes[i-1], &headNodes[i]);
        headNodes[i].item = &heads[i];
    }

    //string nodes
    SetList(&free_nodes, &nodes[0], &nodes[LIST_MAX_NUM_NODES-1], NULL);
    free_nodes.length = LIST_MAX_NUM_NODES;
    JoinEdge(NULL, &nodes[0]);
    nodes[0].item = &nodes[0];
    JoinEdge(&nodes[LIST_MAX_NUM_NODES-1], NULL);
    for(int i = 1; i < LIST_MAX_NUM_NODES; i++)
    {
        JoinEdge(&nodes[i-1], &nodes[i]);
        nodes[i].item = &nodes[i];
    }
    INIT = 1;
}

//set ptr values for a list
static void SetList(List* list, Node* first, Node* last, Node* current)
{
    list->first = first;
    list->last = last;
    list->current = current;
}

//set ptr values for a list, and out of bounds
static void SetListOOB(List* list, Node* first, Node* last, Node* current, enum ListOutOfBounds outofbounds)
{
    list->first = first;
    list->last = last;
    list->current = current;
    list->outofbounds = outofbounds;
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

//add an item to a list with 0 elements
//assumes list has 0 elements
//doesn't set current
static void AddFirstNode(List* list, Node* node)
{
    JoinDoubleEdge(NULL, node, NULL);
    list->first = node;
    list->last = node;
}

//gets a node from the list of free nodes 
//returns NULL on a fail
static Node* GetFreeNode()
{
    if(free_nodes.length == 0)
    {
        return NULL;
    }
    if(free_nodes.length == 1)
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

//pop top element from free_heads stack
static List* GetFreeHead()
{
    if(free_heads.current == NULL)
    {
        return NULL;
    } else
    {
        List* head = free_heads.current->item;
        free_heads.current = free_heads.current->prev;
        ClearList(head);
        return head;
        
    }
}

//push unused head to free_heads stack
static void ReturnHead(List* head)
{
    if(free_heads.current == NULL)
    {
        free_heads.current = free_heads.first;
    } else
    {
        free_heads.current = free_heads.current->next;
    }
    free_heads.current->item = head;
}


//*********************************//
//********  Core Functions ********//
//*********************************//

List* List_create()
{
    if(!INIT)
    {
        Init();
    }
    return GetFreeHead();
}

int List_count(List* pList)
{
    return pList->length;
}

void* List_first(List* pList)
{
    pList->current = pList->first;
    return pList->first->item;
}

void* List_last(List* pList)
{
    pList->current = pList->last;
    return pList->last->item;
}

void* List_next(List* pList)
{   
    //if current before first
    if(pList->length > 0 && pList->current == NULL && pList->outofbounds == LIST_OOB_START)
    {
        pList->current = pList->first;
    }
    //if current in list
    else if(pList->current != NULL)
    {
        pList->current = pList->current->next;
        if(pList->current == NULL)
        {
            pList->outofbounds = LIST_OOB_END;
        }
    }
    return pList->current;
}

void* List_prev(List* pList)
{  
    //if current after last
    if(pList->length > 0 && pList->current == NULL && pList->outofbounds == LIST_OOB_END)
    {
        pList->current = pList->last;
    }
    //if current in list
    else if(pList->current != NULL)
    {
        pList->current = pList->current->prev;
        if(pList->current == NULL)
        {
            pList->outofbounds = LIST_OOB_START;
        }
    }
    return pList->current;
}

void* List_curr(List* pList)
{
    return pList->current;
}

int List_add(List* pList, void* pItem)
{
    Node* node = GetFreeNode();
    if(!node) return -1;
    node->item = pItem;

    //first element in list
    if(pList->length == 0)
    {
        AddFirstNode(pList, node);
        pList->current = node;
    }
    //current is before first
    else if(pList->current == NULL && pList->outofbounds == LIST_OOB_START)
    {
        JoinDoubleEdge(NULL, node,pList->first);
        pList->current = node;
        pList->first = node;
    }
    //current is last or after last
    else if((pList->current == NULL && pList->outofbounds == LIST_OOB_END) || pList->current == pList->last)
    {
        JoinDoubleEdge(pList->last, node, NULL);
        pList->current = node;
        pList->last = node;
    }
    //current is first or somewhere in the middle
    else 
    {
        JoinDoubleEdge(pList->current, node, pList->current->next);
        pList->current = node;
    }

    pList->length++;
    return 0;
}

int List_insert(List* pList, void* pItem)
{
    Node* node = GetFreeNode();
    if(!node) return -1;
    node->item = pItem;

    //first element in list
    if(pList->length == 0)
    {
        AddFirstNode(pList, node);
        pList->current = node;
    }
    //current is after last
    else if(pList->current == NULL && pList->outofbounds == LIST_OOB_END)
    {
        JoinDoubleEdge(pList->last, node, NULL);
        pList->current = node;
        pList->last = node;
    }
    //current is first or before first
    else if((pList->current == NULL && pList->outofbounds == LIST_OOB_START) || pList->current == pList->first)
    {
        JoinDoubleEdge(NULL, node, pList->first);
        pList->current = node;
        pList->first = node;
    }
    //current is last or somewhere in the middle
    else 
    {
        JoinDoubleEdge(pList->current->prev, node, pList->current);
        pList->current = node;
    }

    pList->length++;
    return 0;
}

int List_append(List* pList, void* pItem)
{
    Node* node = GetFreeNode();
    if(!node) return -1;
    node->item = pItem;


    //first element in list
    if(pList->length == 0)
    {
        AddFirstNode(pList, node);
        pList->current = node;
    }
    //after last
    else {
        JoinDoubleEdge(pList->last, node, NULL);
        pList->last = node;
        pList->current = node;
    }
    pList->length++;
    return 0;
}

int List_prepend(List* pList, void* pItem)
{
    Node* node = GetFreeNode();
    if(!node) return -1;
    node->item = pItem;

    //first element in list
    if(pList->length == 0)
    {
        AddFirstNode(pList, node);
        pList->current = node;
    }
    //before first
    else {
        JoinDoubleEdge(NULL, node, pList->first);
        pList->first = node;
        pList->current = node;
    }
    pList->length++;
    return 0;
}

void* List_remove(List* pList)
{
    if(pList->current == NULL)
    {
        return NULL;
    }
    void* deleted_item = pList->current->item;
    Node* deleted_node = pList->current;

    if(pList->first == pList->current)
    {
        pList->first = pList->current->next;
    }

    if(pList->last == pList->current)
    {
        pList->last = pList->current->prev;
    }

    pList->current = pList->current->next;
    if(pList->current == NULL)
    {
        pList->outofbounds = LIST_OOB_END;
    }
    
    JoinEdge(deleted_node->prev, deleted_node->next);
    ReturnNode(deleted_node);
    pList->length--;
    return(deleted_item);
}

void List_concat(List* pList1, List* pList2)
{
    if(pList2->length > 0){
        JoinEdge(pList1->last,pList2->first);
        pList1->last = pList2->last;

        if(pList1->length == 0){
            pList1->first = pList2->first;
            pList1->outofbounds = LIST_OOB_START;
        }
        pList1->length += pList2->length;
        ReturnHead(pList2);
    }
}

typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn)
{
    Node* node = pList->first;
    while(node != NULL)
    {
        (*pItemFreeFn)(node->item);
        Node* next_node = node->next;
        ReturnNode(node);
        node = next_node;
    }
    ReturnHead(pList);
}

void* List_trim(List* pList)
{
    if(pList->length == 0)
    {
        return NULL;
    }
    pList->current = pList->last;
    void* item = List_remove(pList);
    pList->current = pList->last;
    return item;
}

typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg);