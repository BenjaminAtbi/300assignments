

#include <stddef.h>

#ifndef LIST_H
#define LIST_H

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
extern int INIT;

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

void *ListFirst(struct LIST *list); 

void *ListLast(struct LIST *list);

void *ListNext(struct LIST *list);

void *ListPrev(struct LIST *list);

void *ListCurr(struct LIST *list);

int ListAdd(struct LIST *list, void *item);

int ListInsert(struct LIST *list, void *item);

int ListAppend(struct LIST *list, void *item);

int ListPrepend(struct LIST *list, void *item);

void *ListRemove(struct LIST *list);

void ListConcat(struct LIST *list1, struct LIST *list2);

void ListFree(struct LIST *list, void (*itemFree)(void *));

void *ListTrim(struct LIST *list);

void *ListSearch(struct LIST *list, int (*comparator)(void *, void *), void *comparisonArg);


#endif
