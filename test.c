
#include "list.h"
#include <stdio.h>



/* 
1. test reset list
2. test isnodeavailable
3. test Deallocatenode
4. test Allocate Node
5. test addtoempty
6. test listappend
7. test initlists
*/

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

void testIsNodeAvailable()
{
	printf("test IsNodeAvailable\n");
	ResetList(&empty_nodes);
	int pass = 0;
	empty_nodes.length = MAX_NODES - MAX_LISTS;
	if(IsNodeAvailable())
	{
		printf("failed\n");
		return;	
	}
	empty_nodes.length = MAX_NODES - MAX_LISTS + 1;
	if(IsNodeAvailable())
	{
		printf("failed\n");
		return;	
	}
	empty_nodes.length = MAX_NODES - MAX_LISTS - 1;
	if(!IsNodeAvailable())
	{
		printf("failed\n");
		return;	
	}
	printf("passed\n");
	return;	
}	

void testDeallocateNode()
{
	printf("test DeallocateNode");
	ResetList(&empty_nodes);
	DeallocateNode(&nodes[2]);
	DeallocateNode(&nodes[1]);
	DeallocateNode(&nodes[0]);
}	

int main()
{

	printf("testtime\n");
	test_reset();	
	testIsNodeAvailable();
	testDeallocateNode();		
	return 0;
}




