
#include "list.h"

struct Node nodes[LIST_MAX_NUM_NODES];
struct List heads[LIST_MAX_NUM_HEADS];
struct List free_nodes = { 0, NULL, NULL, NULL};
struct List free_heads = { 0, NULL, NULL, NULL};


//*********************************//
//*******  Util Functions  ********//
//*********************************//

//place unallocated heads and nodes into lists
static void Init()
{
    
    //string heads
    free_heads->first = heads[0];
    free_heads->last = heads[LIST_MAX_NUM_HEADS];
    free_heads->length = LIST_MAX_NUM_HEADS;
    JoinEdge(NULL, &heads[0]);
    JoinEdge(heads[LIST_MAX_NUM_HEADS], NULL);
    for(int i = 1; i < LIST_MAX_NUM_HEADS; i++)
    {
        JoinNodes(&heads[i-1], &heads[i])
    }

    //string nodes
    free_nodes->first = nodes[0];
    free_nodes->last = nodes[LIST_MAX_NUM_NODES];
    free_nodes->length = LIST_MAX_NUM_NODES
    JoinEdge(NULL, &nodes[0]);
    JoinEdge(&nodes[LIST_MAX_NUM_NODES], NULL);
    for(int i = 1; i < LIST_MAX_NUM_NODES; i++)
    {
        JoinNodes(&nodes[i-1], &nodes[i])
    }
}

// resets all list values
static void ClearList(struct List* list)
{
    list->len = 0;
    list->first = NULL;
    list->last = NULL;
    list->current = NULL;
}

// resets all node values
static void ClearNode(struct Node* node)
{
    node->item = NULL;
    node->next = NULL;
    node->prev = NULL;
}

// connect two nodes by shared edge
static void JoinEdge(struct Node* left, struct Node* right)
{
    if(left) left->next = right;
    if(right) right->prev = left;
}

// connect a node with two others
static void JoinDoubleEdge(struct Node* left, struct Node* node, struct Node* right)
{
    JoinEdge(left, node);
    JoinEdge(node, right);
}

//******** List functions *********//

//place a new node in a list
// * list: list to place element in
// * ref: node in list to place node becide
// * node: node to be placed
// * relation: which side of the reference node to place new node
static void PlaceNode(struct List* list, struct Node* Ref, struct Node* node, int relation)
{

}

static void AddFirstItem(struct List* list, void *item)
{}


//*********************************//
//********  Core Functions ********//
//*********************************//


List* List_create();

int List_count(List* pList);

void* List_first(List* pList);

void* List_last(List* pList); 

void* List_next(List* pList);

void* List_prev(List* pList);

void* List_curr(List* pList);

int List_add(List* pList, void* pItem);

int List_insert(List* pList, void* pItem);

int List_append(List* pList, void* pItem);

int List_prepend(List* pList, void* pItem);

void* List_remove(List* pList);

void List_concat(List* pList1, List* pList2);

typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn);

void* List_trim(List* pList);

typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg);