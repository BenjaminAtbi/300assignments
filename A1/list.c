
#include "list.h"

struct LIST heads[MAX_LISTS];
struct NODE nodes[MAX_NODES];
struct LIST empty_heads = { 0, 0, NULL, NULL, NULL};
struct LIST empty_nodes = { 0, 0, NULL, NULL, NULL};

int INIT = 0;

//****************************
//* Helper Functions
//****************************

void ResetList(struct LIST *list)
{
	list->length = 0;
	list->cur_state = 0;
	list->first = NULL;
	list->last = NULL;
	list->current = NULL;
}

int AddToEmpty(struct LIST *list, void *item)
{
	list->current = AllocateNode();
	list->current->next = NULL;
	list->current->previous = NULL;
	list->current->item = item;
	list->first = list->current;
	list->last = list->current;
	list->cur_state = 0;
	list->length++;
	return 0;
}

void DeallocateNode(struct NODE *node)
{
	node->item = NULL;
	empty_nodes.current = node;		
	
	if(ListCount(&empty_nodes) <= 0)
	{
		empty_nodes.first = node;
		empty_nodes.last = node;
		empty_nodes.current->previous = NULL;
		empty_nodes.current->next = NULL;
		empty_nodes.length++;
		return;
	}

	empty_nodes.current->next = empty_nodes.first;
	empty_nodes.current->previous = NULL;
	empty_nodes.first->previous = empty_nodes.current;
	empty_nodes.first = empty_nodes.current;
	empty_nodes.length++;
	return;
}	

struct NODE *AllocateNode()
{
	struct NODE *node = NULL;
	if(ListCount(&empty_nodes) > 1)
	{
		struct NODE *node = empty_nodes.last;		
		empty_nodes.last = empty_nodes.last->previous;
		empty_nodes.last->next = NULL;
		empty_nodes.length--;
		return node;		
	}
	else if(ListCount(&empty_nodes) == 1)
	{
		struct NODE *node = empty_nodes.last;		
		empty_nodes.last = NULL;
		empty_nodes.current = NULL;
		empty_nodes.first = NULL;
		empty_nodes.length--;
		return node;
	}
	else {
		return NULL;
	}
}


int IsNodeAvailable()
{
	int headSpace = MAX_LISTS - ListCount(&empty_heads);
	return ListCount(&empty_nodes) > headSpace;
}

void InitLists()
{
	INIT = 1;
	ResetList(&empty_heads);
	ResetList(&empty_nodes);

	for(int i = 0; i < MAX_NODES; i++)
	{
		DeallocateNode(&nodes[i]);
	}

	for(int i = 0; i < MAX_LISTS; i++)
	{	
		ResetList(&heads[i]);	
		ListAppend(&empty_heads, &heads[i]);
	}
}
			

//****************************
//* Core Functions
//****************************

struct LIST *ListCreate()
{
	if(!INIT)
	{
		InitLists();
	}

	int emptyHeadCount = ListCount(&empty_heads);
	if(emptyHeadCount <= 0 || emptyHeadCount > MAX_LISTS) return NULL;
	struct LIST *list = ListTrim(&empty_heads);
	ResetList(list);
	return list;			
}

int ListCount(struct LIST *list)
{
	return list->length;
}

void *ListFirst(struct LIST *list)
{
	list->current = list->first;
	list->cur_state = 0;
	if(list->current == NULL) return NULL;
	else return list->current->item;	
}

void *ListLast(struct LIST *list)
{
	list->current = list->last;
	list->cur_state = 0;
	if(list->current == NULL) return NULL;
	else return list->current->item;
}

void *ListNext(struct LIST *list)
{
	if(list->length <= 0 || list->cur_state == 1)
	{
		return NULL;
	} 
	else if(list->cur_state == -1) 
	{
		list->current = list->first;
		list->cur_state = 0;
		return list->current->item;		
	} 
	else if (list->current == list->last)
	{
		list->current = NULL;
		list->cur_state = 1;
		return NULL;	
	} else 
	{
		list->current = list->current->next;
		return list->current->item;
	}
}

void *ListPrev(struct LIST *list)
{
	if(list->length <= 0 || list->cur_state == -1)
	{
		return NULL;
	} 
	else if(list->cur_state == 1) 
	{
		list->current = list->last;
		list->cur_state = 0;
		return list->current->item;		
	} 
	else if (list->current == list->first)
	{
		list->current = NULL;
		list->cur_state = -1;
		return NULL;	
	} else 
	{
		list->current = list->current->previous;
		return list->current->item;
	}
}

void *ListCurr(struct LIST *list)
{
	if(list->current == NULL) return NULL;
	else return list->current->item;
}

int ListAdd(struct LIST *list, void *item)
{
	if(!IsNodeAvailable())
	{
		return -1;
	}

	if(ListCount(list) <= 0)
	{	
		return AddToEmpty(list, item);		
	}

	if(list->cur_state == 1 || list->current == list->last)
	{
		return ListAppend(list, item);
	}

	if(list->cur_state == -1)
	{
		return ListPrepend(list, item);
	}
	
	if(list->cur_state == 0)
	{
		struct NODE *new_next = list->current->next;
		list->current->next = AllocateNode();
		list->current->next->previous = list->current;
		list->current->next->next = new_next;
		list->current = list->current->next;
		new_next->previous = list->current;
		list->current->item = item;
		list->cur_state = 0;
		list->length++;	
		return 0;
	}
	return -1;
}


int ListInsert(struct LIST *list, void *item)
{
	if(!IsNodeAvailable())
	{
		return -1;
	}

	if(ListCount(list) <= 0)
	{	
		return AddToEmpty(list, item);		
	}

	if(list->cur_state == -1 || list->current == list->first)
	{
		return ListPrepend(list, item);
	}

	if(list->cur_state == 1)
	{
		return ListAppend(list, item);
	}
	
	if(list->cur_state == 0)
	{
		struct NODE *new_prev = list->current->previous;
		list->current->previous = AllocateNode();
		list->current->previous->next = list->current;
		list->current->previous->previous = new_prev;
		list->current = list->current->previous;
		new_prev->next = list->current;
		list->current->item = item;
		list->cur_state = 0;
		list->length++;	
		return 0;
	}
	return -1;
}

int ListAppend(struct LIST *list, void *item)
{
	if(!IsNodeAvailable())
	{
		return -1;
	}

	if(ListCount(list) <= 0)
	{
		return AddToEmpty(list, item);		
	}

	list->current = AllocateNode();
	list->current->previous = list->last;
	list->current->next = NULL;
	list->last->next = list->current;
	list->last = list->current;
	list->current->item = item;
	list->cur_state = 0;
	list->length++;	
	return 0;
}

int ListPrepend(struct LIST *list, void *item)
{
	if(!IsNodeAvailable())
	{
		return -1;
	}

	if(ListCount(list) <= 0)
	{
		return AddToEmpty(list, item);		
	}
	
	list->current = AllocateNode();
	list->current->previous = NULL;
	list->current->next = list->first;
	list->first->previous = list->current;
	list->first = list->current;
	list->current->item = item;
	list->cur_state = 0;
	list->length++;	
	return 0;
}	

void *ListRemove(struct LIST *list)
{
	if(list->current == NULL) return NULL;
	void *val = ListCurr(list);
	if(list->current->next != NULL) list->current->next->previous = list->current->previous;
	if(list->current->previous != NULL) list->current->previous->next = list->current->next;
	if(list->current == list->last) list->cur_state = 1;
	struct NODE *node = list->current;
	list->current = list->current->next;
	DeallocateNode(node);
	list->length--;
	return val;
}

void ListConcat(struct LIST *list1, struct LIST *list2)
{
	if(ListCount(list1) > 0)
	{
		list1->last->next = list2->first;
		if(ListCount(list2) > 0)
		{
			list2->first->previous	= list1->last;
		}
	}
	else 
	{
		list1->first = list2->first;
		list1->current = NULL;
		list1->cur_state = -1;
	}

	if(ListCount(list2) > 0) list1->last = list2->last;
	
	list1->length += list2->length;
	ListAppend(&empty_heads, list2);
}

void ListFree(struct LIST *list, void (*itemFree)(void *))
{
	struct NODE *node_cur;
	node_cur = list->first;
	while(node_cur != NULL)
	{
		(*itemFree)(node_cur->item);
		node_cur = node_cur->next;
	}
	ListAppend(&empty_heads, list);
}

void *ListTrim(struct LIST *list)
{

	if(ListCount(list) <= 0)
	{
		return NULL;
	}
	void *val = list->last->item;

	if(ListCount(list) > 1)
	{
		list->current = list->last->previous;
		list->current->next = NULL;
		DeallocateNode(list->last);
		list->last = list->current;
	}
	else if(ListCount(list) == 1)
	{
		DeallocateNode(list->last);
		list->current = NULL;
		list->last = NULL;
		list->first = NULL;
	} 
		
	list->cur_state = 0;
	list->length = list->length - 1;
	return val;
}

void *ListSearch(struct LIST *list, int (*comparator)(void *, void *), void *comparisonArg)
{
	while(list->current != NULL)
	{
		if (comparator(list->current->item, comparisonArg))
		{
			return list->current->item;
		}
		list->current = list->current->next;
	}
	list->cur_state = 1;
	return NULL;
}
