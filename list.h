
#include <stddef.h>

struct NODE
{
	void *item;
	struct NODE *next;
	struct NODE *previous;
};

struct LIST
{
	int length;
	int cur_state;
	struct NODE *first;
	struct NODE *last;
	struct NODE *current;
};

#define MAX_LISTS 10
#define MAX_NODES 100

extern struct LIST heads[MAX_LISTS];
extern struct NODE nodes[MAX_NODES];
extern struct LIST empty_heads;
extern struct LIST empty_nodes;


//*****************
// Helper Function decs
//******************

void ResetList(struct LIST *list);

int AddToEmpty(struct LIST *list, void *item);

void DeallocateNode(struct NODE *node);

struct NODE *AllocateNode();

int IsNodeAvailable();

void InitLists();

//******************
// Core Function decs
//******************

struct LIST *ListCreate();

int ListCount(struct LIST *list);

//void *ListFirst(list); 

//void *ListLast(list);

//void *ListNext(list);

//void *ListPrev(list);

//void *ListCurr(list);

int ListAdd(struct LIST *list, void *item);

//int ListInsert(list, item);

int ListAppend(struct LIST *list, void *item);

int ListPrepend(struct LIST *list, void *item);

//void *ListRemove(list);

//void ListConcat(list1, list2);

//void ListFree(list, itemFree);

void *ListTrim(struct LIST *list);

//void *ListSearch(list, comparator, comparisonArg);
