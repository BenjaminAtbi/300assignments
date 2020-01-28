
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

struct LIST *ListCreate();

//int ListCount(list);
//
//void *ListFirst(list); 
//
//void *ListLast(list);
//
//void *ListNext(list);
//
//void *ListPrev(list);
//
//void *ListCurr(list);
//
//int ListAdd(list, item);
//
//int ListInsert(list, item);
//
//int ListAppend(list, item);
//
//int ListPrepend(list, item);
//
//void *ListRemove(list);
//
//void ListConcat(list1, list2);
//
//void ListFree(list, itemFree);
//
//void *ListTrim(list);
//
//void *ListSearch(list, comparator, comparisonArg);
