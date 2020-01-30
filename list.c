
/* array of LIST* for list heads
 * array of NODE* for nodes
 * LIST of empty nodes and LIST of empty heads
 * adding takes node from start of empty list
 * deleting adds node to end 
 *
*/

#include "list.h"

struct LIST heads[MAX_LISTS];
struct NODE nodes[MAX_NODES];
struct LIST empty_heads;
struct LIST empty_nodes;

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
		node = empty_nodes.last;		
		empty_nodes.last = empty_nodes.last->previous;
		empty_nodes.last->next = NULL;	
	}
	if(ListCount(&empty_nodes) == 1)
	{
		node = empty_nodes.last;		
		empty_nodes.last = NULL;
		empty_nodes.current = NULL;
		empty_nodes.last = NULL;
	}

	return node;
}

int IsNodeAvailable()
{
	return (ListCount(&empty_nodes) > MAX_NODES - MAX_LISTS);
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
		ListAppend(&empty_heads, &heads[i]);
	}
	//*******************
	// allocate list of heads
}
			

//****************************
//* Core Functions
//****************************

/* allocate a new list from empty_heads
 * on first run, push all nodes into empty_nodes,
 * then allocate and push list of heads into empty_heads
*/
struct LIST *ListCreate()
{
	if(!INIT)
	{
		InitLists();
	}

	//****************
	// temp return					
	return &empty_heads;			
}

int ListCount(struct LIST *list)
{
	return list->length;
}


/* check empty_nodes vs max nodes
 * first/last null -> empty -> stick it in
 * cur before -> ListPrepend
 * cur after -> ListAppend
 * cur middle -> stick it in
*/
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

	//current is past last, current is last, or only one element
	if(list->cur_state == 1 || list->current == list->last)
	{
		return ListAppend(list, item);
	}

	//current is before first	
	if(list->cur_state == -1)
	{
		return ListPrepend(list, item);
	}
	
	//current is first or somewhere within
	if(list->cur_state == 0)
	{
		struct NODE *new_next = list->current->next;
		list->current->next = AllocateNode();
		list->current->next->previous = list->current;
		list->current->next->next = new_next;
		list->current = list->current->next;
		list->current->item = item;
		list->cur_state = 0;
		list->length++;	
		return 0;
	}
	return -1;
}

/* check for available node
 * check if empty
 * stick after last
*/
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
	
	

void *ListTrim(struct LIST *list)
{

	if(ListCount(list) >= 1)
	{
		void *val = list->last->item;
		list->current = list->last->previous;
		list->current->next = NULL;
		DeallocateNode(list->last);
		list->last = list->current;
		
		if(ListCount(list) == 1)
		{
			list->first = NULL;
		} 
		
		list->cur_state = 0;
		list->length = list->length - 1;
	}
	return NULL;
}



