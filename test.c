#include "list.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>

struct NODE *ptrlist[MAX_NODES];

void ResetTestEnv(){
	resetPtrList();
	ResetList(&empty_nodes);
	ResetList(&empty_heads);	
	INIT = 0;		
}

void resetPtrList()
{
	for(int i = 0; i < MAX_NODES; i++)
	{
		ptrlist[i] = NULL;
	}
}

int PtrId(struct NODE *ptr)
{
	if(ptr == NULL) return -1;
	for(int i = 0; i < MAX_NODES; i++){
		if(ptr == ptrlist[i])
		{
			return i;
		}
		if(ptrlist[i] == NULL)
		{
			ptrlist[i] = ptr;
			return i;
		}
	}
	return -1;		
}

void printList(struct LIST *list)
{
	printf("length: %i, cur_state: %i, current: %i\n", list->length, list->cur_state, PtrId(list->current));	
	int i = 0;
	struct NODE *node_cur;
	node_cur = list->first;
	printf("first -> %i | ", PtrId(list->first));	
	while(node_cur != NULL)
	{
		printf(" | %i <- %i -> %i | ", PtrId(node_cur->previous), PtrId(node_cur), PtrId(node_cur->next));
		node_cur = node_cur->next; 
	}
	printf(" | %i <- last\n",  PtrId(list->last));

}

void printIntList(struct LIST *list)
{
	printf("int contents of list: ");
	struct NODE *node_cur;
	node_cur = list->first;
	while(node_cur != NULL)
	{
		printf("%i, ", *(int*)node_cur->item);
		node_cur = node_cur->next; 
	}
	printf("\n");
}

void test_reset()
{
	printf("test ResetList\n");
	struct LIST lt;
	ResetList(&lt);
	if(lt.length == 0 && lt.cur_state == 0 && lt.first == NULL && lt.last == NULL && lt.current == NULL)
	{
		printf("passed\n");
		return;
	}
	printf("failed\n");
}

void testAllocation()
{
	printf("test DeallocateNode\n");
	ResetList(&empty_nodes);
	ResetList(&empty_heads);
	for(int i = 0; i < 6; i++){
		DeallocateNode(&nodes[i]);
	}
	printf("set 6 new nodes in empty_nodes\n");	
	printf("state of empty_nodes --- ");
	printList(&empty_nodes);
	printf("allocating 3\n");
	struct NODE *val[6];
	val[0] = AllocateNode();
	val[1] = AllocateNode();
	val[2] = AllocateNode();
	if(val[0] != 0 && val[1] != 0 && val[2] != 0) printf("nodes aren't null: true\n");
	else printf("nodes aren't null: false\n");
	printf("state of empty_nodes --- ");
	printList(&empty_nodes);		
	printf("allocating 3 more\n");
	val[3] = AllocateNode();
	val[4] = AllocateNode();
	val[5] = AllocateNode();
	if(val[3] != 0 && val[4] != 0 && val[5] != 0) printf("nodes aren't null: true\n");
	else printf("nodes aren't null: false\n");
	printf("state of empty_nodes: ---");
	printList(&empty_nodes);		
}

void testAddToEmpty()
{
	printf("test AddToEmpty\n");
	ResetList(&empty_nodes);
	ResetList(&empty_heads);	
	for(int i = 0; i < 6; i++){
		DeallocateNode(&nodes[i]);
	}
	printf("adding to empty list\n");
	int val = 4;
	AddToEmpty(&empty_heads, &val);
	printf("state of list ---- ");
	printList(&empty_heads);
	if(empty_heads.current->item == &val) printf("val was stored: true\n");
	else printf("val was stored: false\n");
}

void testListAppend()
{
	printf("test ListAppend\n");
	ResetTestEnv();	
	InitLists();
	ResetList(&empty_heads);
	printf("values: 1, 2, 3\n");
	int vals[3] = {1,2,3};
	ListAppend(&empty_heads, &vals[0]);
	printf("state after append: --- ");
	printList(&empty_heads);
	ListAppend(&empty_heads, &vals[1]);
	printf("state after append: --- ");
	printList(&empty_heads);
	ListAppend(&empty_heads, &vals[2]);
	printf("state after append: --- ");
	printList(&empty_heads);
	printf("values: %i, %i, %i\n", *(int*)empty_heads.first->item, *(int*)empty_heads.first->next->item, *(int*)empty_heads.first->next->next->item);
}

void testListCount()
{
	printf("test ListCount\n");
	ResetTestEnv();
	empty_nodes.length = 6;
	if (ListCount(&empty_nodes) == 6) printf("test passed: true\n");
	else printf("test passed: false\n");
}


void testInitLists()
{
	printf("test InitLists\n");
	ResetTestEnv();
	InitLists();
	printf("empty_nodes length: %i\n", ListCount(&empty_nodes));
	printf("state of empty_heads:  --- ");
	printList(&empty_heads);
	printf("state of each head in empty_heads: --- \n");
	struct NODE *iter = empty_heads.first;
	for(int i = 0; i < ListCount(&empty_heads); i++)
	{
	//	printList((struct LIST*)(iter->item));
	//	iter = iter->next;
		printList(&heads[i]);
	}
	
}

void testListTrim()
{
	printf("test List Trim\n");
	ResetTestEnv();
	InitLists();
	ResetList(&empty_heads);
	int a = 1;
	int b = 2;
	int c = 3;
	ListAppend(&empty_heads, &a);
	ListAppend(&empty_heads, &b);
	ListAppend(&empty_heads, &c);
	printf("gave list 1,2,3\nstate of list --- ");
	printList(&empty_heads);
	printf("trim: %i\nstate of list --- ", *(int*) ListTrim(&empty_heads));
	printList(&empty_heads);
	printf("trim: %i\nstate of list --- ", *(int*) ListTrim(&empty_heads));
	printList(&empty_heads);
	printf("trim: %i\nstate of list --- ", *(int*) ListTrim(&empty_heads));
	printList(&empty_heads);
}

void testListCreate()
{
	printf("test List Create\n");
	ResetTestEnv();
	InitLists();
	struct LIST *list1 = ListCreate();
	
	struct LIST *list2 = ListCreate();
	printf("state of empty_heads --- ");
	printList(&empty_heads);
	printf("state of list 1 --- ");
	printList(list1);
	printf("state of list 2 --- ");
	printList(list2);
}
	
void testListPrepend()
{
	printf("test Prepend\n");
	ResetTestEnv();
	struct LIST *list = ListCreate();
	printf("prepending 1, 2, 3\n");
	int nums[] = {1,2,3};	
	ListPrepend(list,&nums[0]);
	ListPrepend(list,&nums[1]);
	ListPrepend(list,&nums[2]);
	printList(list);
	printIntList(list);	
}	

void testFirstLast()
{
	printf("test First Last\n");
	ResetTestEnv();
	struct LIST *list = ListCreate();
	printf("appending 1, 2, 3\n");
	int nums[] = {1,2,3};	
	ListAppend(list,&nums[0]);
	ListAppend(list,&nums[1]);
	ListAppend(list,&nums[2]);
	printf("state of list --- ");
	printList(list);
	printf("get first: %i\n", *(int*)ListFirst(list));
	printf("state of list --- ");
	printList(list);
	printf("get last: %i\n", *(int*)ListLast(list));
	printf("state of list --- ");
	printList(list);
}

void testNextPrev()
{
	printf("test Next Prev\n");
	ResetTestEnv();
	struct LIST *list = ListCreate();
	printf("appending 1, 2, 3\n");
	int nums[] = {1,2,3};	
	ListAppend(list,&nums[0]);
	ListAppend(list,&nums[1]);
	ListAppend(list,&nums[2]);
	printf("state of list --- ");
	printList(list);

	printf("step off list val: ");
	int *val = (int*)ListNext(list);
	if (val == NULL) { printf("NULL --- "); } else {printf(" %i --- ",*val); }	
	printList(list);	
	val = (int*)ListPrev(list);

	printf("step back on list val: ");
	if (val == NULL) { printf("NULL --- "); } else {printf(" %i --- ",*val); }
	printList(list);

	printf("step back to first val: ");
	val = (int*)ListPrev(list);
	if (val == NULL) { printf("NULL, "); } else {printf(" %i, ",*val); }
	val = (int*)ListPrev(list);
	if (val == NULL) { printf("NULL --- "); } else {printf(" %i --- ",*val); }
	printList(list);
	
	val = (int*)ListPrev(list);
	printf("step before list val: ");
	if (val == NULL) { printf("NULL --- "); } else {printf(" %i --- ",*val); }
	printList(list);

	val = (int*)ListNext(list);
	printf("step back on list val: ");
	if (val == NULL) { printf("NULL --- "); } else {printf(" %i --- ",*val); }
	printList(list);

}	

void testAddInsert()
{
	printf("test Add Insert\n");
	ResetTestEnv();
	struct LIST *list = ListCreate();
	printf("appending 1, 2, 3, move to middle\n");
	int nums[] = {1,2,3,8,9};	
	ListAppend(list,&nums[0]);
	ListAppend(list,&nums[1]);
	ListAppend(list,&nums[2]);
	ListPrev(list);
	printf("state of list --- ");
	printList(list);
	printf("add 8 -- success: %i\n", ListAdd(list, &nums[3]));
	printList(list);
	printIntList(list);
	ListPrev(list);
	printf("move back then Insert 9 -- success: %i\n", ListInsert(list, &nums[4]));
	printList(list);
	printIntList(list);
}	

void testRemove()
{
	printf("test Remove\n");
	ResetTestEnv();
	struct LIST *list = ListCreate();
	printf("appending 1, 2, 3, move to middle\n");
	int nums[] = {1,2,3};	
	ListAppend(list,&nums[0]);
	ListAppend(list,&nums[1]);
	ListAppend(list,&nums[2]);
	ListPrev(list);
	printf("remove once val: %i -- ",*(int*)ListRemove(list));
	printList(list);
	printf("remove once val: %i -- ",*(int*)ListRemove(list));
	printList(list);
}	

void testConcat()
{
	printf("test Concat\n");
	ResetTestEnv();
	struct LIST *list1 = ListCreate();
	struct LIST *list2 = ListCreate();	
	printf("put 3 in each\n");
	int nums[] = {1,2,3,4,5,6};	
	ListAppend(list1,&nums[0]);
	ListAppend(list1,&nums[1]);
	ListAppend(list1,&nums[2]);
	ListAppend(list2,&nums[3]);
	ListAppend(list2,&nums[4]);
	ListAppend(list2,&nums[5]);
	printf("list 1 -- ");
	printList(list1);
	printf("list 2 -- ");
	printList(list2);
	printf("concat -- ");
	ListConcat(list1, list2);
	printList(list1);
	struct LIST *list3 = ListCreate();
	printf("concat with empty -- ");
	ListConcat(list1, list3);
	printList(list1);
	struct LIST *list4 = ListCreate();
	printf("concat empty with list -- ");
	ListConcat(list4, list1);
	printList(list4);
}


void testFree()
{
	printf("test Free\n");
	ResetTestEnv();
	int *a = malloc(sizeof(*a));
	int *b = malloc(sizeof(*b));
	int *c = malloc(sizeof(*c));
	struct LIST *list = ListCreate();
	ListAdd(list, a);
	ListAdd(list, b);
	ListAdd(list, c);
	printf("list of 3 things --- ");
	printList(list);
	printf("free list\n");
	ListFree(list, &free);
	printf("check up on empty_heads --- ");
	printList(&empty_heads);
}

int IntEqual(void *value, void *target)
{
	return *(int*)value == *(int*)target;
}

void testSearch()
{
	printf("test Search\n");
	ResetTestEnv();
	int nums[] = {1,2,3,4,5,6};
	struct LIST *list = ListCreate();	
	ListAppend(list,&nums[0]);
	ListAppend(list,&nums[1]);
	ListAppend(list,&nums[2]);
	ListAppend(list,&nums[3]);
	ListAppend(list,&nums[4]);
	ListAppend(list,&nums[5]);
	ListFirst(list);
	printf("list of ints, current is first --- ");
	printList(list);
	printIntList(list);
	printf("search for 4\n");
	int four = 4;
	int *ptr = ListSearch(list, &IntEqual, &four);
	printf("address is correct: %i\n", ptr == &nums[3]);
	printf("state of list --- ");
	printList(list);	
}

int main()
{
	//some tests do not reset the state correctly and can break other tests if run in sequence
	
	//test_reset();	
	//testAllocation();		
	//testAddToEmpty();	
	//testListAppend();
	//testListCount();	
	//testInitLists();	
	//testListTrim();
	//testListCreate();
	//testListPrepend();
	//testFirstLast();		
	//testNextPrev();	
	//testAddInsert();	
	//testRemove();
	//testConcat();
	//testFree();
	//testSearch();
	return 0;
}




