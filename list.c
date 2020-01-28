
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

void initList(LIST *list)
{
	list->length = 0;
	list->cur_state = 0;
	list->first = NULL;
	list->last = NULL;
	list->current = NULL;
};

int AddToEmpty(List *list, void *item)
{
	list->current = ListTrim(empty_nodes);
	list->first = list->current;
	list->last = list->current;
	cur_state = 0;
	return 0;
};


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
		INIT = 1;
		for(int i = 0; i < MAX_NODES; i++)
		{
			ListAppend(&empty_nodes, &nodes[i]);
						
	return &empty_heads;			
}

/* check empty_nodes vs max nodes
 * first/last null -> empty -> stick it in
 * cur before -> ListPrepend
 * cur after -> ListAppend
 * cur middle -> stick it in
*/
int ListAdd(LIST *list, void *item)
{
	if(ListCount(empty_nodes) <= 0)
	{
		return -1;
	}

	if(!list->first)
	{
		return AddToEmpty(list, item);		
	}

	//current is past last, current is last, or only one element
	if(cur_state == 1 || list->current == list->last)
	{
		return ListAppend(list, item);
	}
	
	if(cur_state == -1)
	{
		return ListPrepend(list, item);
	}
	
	//current is first or somewhere within
	if(cur_state == 0)
	{
		NODE *new_next = list->current->next;
		list->current->next = ListTrim(empty_nodes);
		list->current->next->previous = list->current;
		list->current->next->next = new_next;
		list->current = list->current->next;
		list->current->item = item;
		
		return 0;
	}
	return -1;
}

/* check for available node
 * check if empty
 * stick after last
*/
int ListAppend(LIST *list, void *item)
{
	if(ListCount(empty_nodes) <= 0)
	{
		return -1;
	}

	if(!list->last)
	{
		return AddToEmpty(list, item);		
	}

	
























