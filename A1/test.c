
#include "list.h"
#include "test.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/***********************
TEMPORARY DECLARATION
*************/ 

// Node nodes[LIST_MAX_NUM_NODES];
// List heads[LIST_MAX_NUM_HEADS];
// List free_nodes = { 0, NULL, NULL, NULL};
// List free_heads = { 0, NULL, NULL, NULL};

/***********************
TEMPORARY DECLARATION
*************/ 

//exit program upon error in list
void listErr(List* list, char* err)
{
    printf("%s",err);
    printList(list);
    exit(1);
}

//exit with non-specific error
void generalErr(char* err)
{
    printf("%s",err);
    exit(1);
}

//prints details regarding list
void printList(List* list)
{
    printHeadDetails(list);
    printf("### First ###\n");
    printNodeDetails(list->first);
    printf("### Last ###\n");
    printNodeDetails(list->last);
    printf("### Current ###\n");
    printNodeDetails(list->current);
    printf("### Traversal ###\n");
    Node* node = list->first;
    for(int i = 0; node && i <= LIST_MAX_NUM_NODES; i++)
    {
        if(i == LIST_MAX_NUM_NODES)
        {
            printf("TRAVERSAL MAXED OUT\n");
            break;
        }
        printNodeDetails(node);
        node = node->next;
    }
}

//prints all details regarding head
void printHeadDetails(List* list)
{
    printf("[[ First: ");
    printNodeSignature(list->first);
    printf(" Last: ");
    printNodeSignature(list->last);
    printf(" Current: ");
    printNodeSignature(list->current);
    printf(" length: %i ]]\n", list->length);
}

//print all details regarding node
void printNodeDetails(Node* node)
{
    if(node == NULL)
    {
        printf("[ NULL ]\n");
    }else 
    {
        printf("[ ");
        printNodeSignature(node->prev);
        printf(" <- ");
        printNodeSignature(node);
        if (node->item) printf(" { %i }", *(int*)node->item);
        else printf(" { NULL }");
        printf(" -> ");
        printNodeSignature(node->next);
        printf(" ]\n");
    }
}

//print minimal representation of node
void printNodeSignature(Node* node)
{
    
    if(node) printf("( ID: %i )",GetNodeID(node));
    else printf("( NULL )");
}

//return ID of a node
// * ID is arbitrary, determined by order id requested
static Node* nodeIDs[LIST_MAX_NUM_NODES];
static int last_id = 0;

void InitNodeIDs()
{
    for(int i = 0; i < LIST_MAX_NUM_NODES; i++)
    {
        nodeIDs[i] = NULL;
    }
}

int GetNodeID(Node* node)
{
    for(int i = 0; i <= last_id; i++)
    {
        if(nodeIDs[i] == node){
            return i;
        }
    }
    nodeIDs[last_id] = node;
    last_id++;
    return last_id - 1;
}

//set first three values of a control list
void setControlList(void** control_list, void* first_item, void* last_item, void* current_item)
{
    control_list[0] = first_item;
    control_list[1] = last_item;
    control_list[2] = current_item;
}

//validate that all aspects of a list are correct
//control list is list of items that list should contain.
//control_list[0-2] are list ptrs first, last, current.
//control_list[3-n] are correct contents of list nodes, in order.
void validateList(List* list, void** control_list)
{
    //basic validation of values
    
    if(list->length == 0 && ((list->first != NULL) || (list->last != NULL)))
    {
        listErr(list, "ValidateList: list of length 0 has nodes\n");
    }
    if(list->length != 0 && ((list->first == NULL) || (list->last == NULL)))
    {
        listErr(list, "ValidateList: list of length >0 has NULL pointer\n");
    }
    if(list->first != NULL && list->first->prev != NULL)
    {
        listErr(list, "ValidateList: first node not preceded by NULL\n");
    }
    //validate list ptrs vs control_list
    if(control_list){
        if(list->first == NULL && control_list[0] != NULL)
        {
            char msg[100];
            sprintf(msg, "ValidateList: item of list.first not correct. Item: %p Control: %p \n", (void*)list->first, (void*)control_list[0]);
            listErr(list, msg);
        } else if(list->first && list->first->item != control_list[0])
        {
            char msg[100];
            sprintf(msg, "ValidateList: item of list.first not correct. Item: %p Control: %p \n", (void*)list->first->item, (void*)control_list[0]);
            listErr(list, msg);
        }
        if(list->last == NULL && control_list[1] != NULL)
        {
            char msg[100];
            sprintf(msg, "ValidateList: item of list.last not correct. Item: %p Control: %p \n", (void*)list->last, (void*)control_list[1]);
            listErr(list, msg);
        } else if(list->last && list->last->item != control_list[1])
        {
            char msg[100];
            sprintf(msg, "ValidateList: item of list.last not correct. Item: %p Control: %p \n", (void*)list->last->item, (void*)control_list[1]);
            listErr(list, msg);
        }
        if(list->current == NULL && control_list[2] != NULL)
        {
            char msg[100];
            sprintf(msg, "ValidateList: item of list.current not correct. Item: %p Control: %p \n", (void*)list->current, (void*)control_list[2]);
            listErr(list, msg);
        } else if(list->current && list->current->item != control_list[2])
        {
            char msg[100];
            sprintf(msg, "ValidateList: item of list.current not correct. Item: %p Control: %p \n", (void*)list->current->item, (void*)control_list[2]);
            listErr(list, msg);
        }
    }
    //validate nodes
    if(list->first != NULL &&list->last != NULL)
    {
        Node* node = list->first;
        for(int i = 0; i < LIST_MAX_NUM_NODES; i++)
        {
            if(control_list && node->item != control_list[i+3])
            {
                char msg[100];
                sprintf(msg, "element %02i incorrect. Item: %p Control: %p \n", i, (void*)node->item, (void*)control_list[i+3]);
                listErr(list, msg);
            }
            if(node->next == NULL)
            {
                if(list->length != i+1){
                    char msg[100];
                    sprintf(msg, "length not correct. Length: %i Elements traversed: %i\n", list->length, i+1);
                    listErr(list, msg);
                }
                break;
            }
            node = node->next;
        }
        if(list->last != node)
        {
            listErr(list, "last element in traversal != last pointer");
        }
    }
}

//initial self-sufficient test, string free nodes head with node 3 and 4
// void test0(){
//     int e1 = 33;
//     int e2 = 44;
//     free_nodes.length = 2;
//     free_nodes.first = &nodes[3];
//     free_nodes.last = &nodes[4];
//     free_nodes.current = &nodes[4];
//     nodes[3].prev = NULL;
//     nodes[3].next = &nodes[4];
//     nodes[3].item = &e1;
//     nodes[4].prev = &nodes[3];
//     nodes[4].next = NULL;
//     nodes[4].item = &e2;
//     printList(&free_nodes);
// }

//test Init 
// void testn3(){
//     printf("commencing testn3: Init function\n");
//     Init();

//     //validate free_nodes
//     void* control_list[LIST_MAX_NUM_NODES+3];
//     setControlList(control_list, &nodes[0], &nodes[LIST_MAX_NUM_NODES-1], NULL);
//     for (int i = 0; i < LIST_MAX_NUM_NODES; i++)
//     {
//         control_list[i+3] = &nodes[i];
//     }
//     validateList(&free_nodes, control_list);
//     printf("free_nodes list correct\n");

//     //validate free_heads
//     void* control_heads[LIST_MAX_NUM_HEADS+3];
//     setControlList(control_heads, &heads[0], &heads[LIST_MAX_NUM_HEADS-1], &heads[LIST_MAX_NUM_HEADS-1]);
//     for (int i = 0; i < LIST_MAX_NUM_HEADS; i++)
//     {
//         control_heads[i+3] = &heads[i];
//     }
//     validateList(&free_heads, control_heads);
//     printf("free_heads list correct\n");
//     printf("testn3 passed: Init function created lists correctly\n");
// }



// //test popping and returning heads
// void testn2(){
//     printf("commencing testn2: getting and returning heads\n");
//     Init();
//     //get all heads in order
//     List* head_bank[LIST_MAX_NUM_HEADS];
//     for(int i = 0; i < LIST_MAX_NUM_HEADS; i++)
//     {
//         head_bank[i] = GetFreeHead();
//     }
//     void* control_list[LIST_MAX_NUM_HEADS+3];
//     setControlList(control_list, &heads[0], &heads[LIST_MAX_NUM_HEADS-1], NULL);
//     for(int i = 0; i < LIST_MAX_NUM_HEADS; i++)
//     {
//         control_list[i+3] = &heads[i];
//     }
//     validateList(&free_heads, control_list);
//     printf("heads allocated correctly\n");

//     //return in reverse order
//     for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){
//         ReturnHead(head_bank[i]);
//     }
//     setControlList(control_list, &heads[LIST_MAX_NUM_HEADS-1], &heads[0], &heads[0]);
//     for(int i = 0; i < LIST_MAX_NUM_HEADS; i++){
//         control_list[i+3] = &heads[LIST_MAX_NUM_HEADS-1-i];
//     }
//     validateList(&free_heads, control_list);
//     printf("heads returned correctly\n");
//     printf("testn2 passed: heads managed correctly\n");
// }

// //test getting and replacing nodes
// void testn1(){
//     printf("commencing testn1: getting and returning nodes\n");
//     Init();

//     //get all nodes in order
//     Node* node_bank[LIST_MAX_NUM_NODES];
//     for(int i = 0; i < LIST_MAX_NUM_NODES; i++)
//     {
//         node_bank[i] = GetFreeNode();
//     }
//     void* control_list[3];
//     setControlList(control_list, NULL, NULL, NULL);
//     validateList(&free_nodes, control_list);
//     printf("nodes allocated correctly\n");

//     int vals[LIST_MAX_NUM_NODES];
//     void* control_nodes[LIST_MAX_NUM_NODES+3];
//     setControlList(control_nodes, &vals[0], &vals[LIST_MAX_NUM_NODES-1], NULL);
//     for(int i = 0; i < LIST_MAX_NUM_NODES; i++)
//     {
//         vals[i] = i;
//         node_bank[i]->item = &vals[i];
//         control_nodes[i+3] = &vals[i];
//         ReturnNode(node_bank[i]);
//     }
//     validateList(&free_nodes, control_nodes);
//     printf("nodes returned correctly\n");
//     printf("testn1 passed: nodes managed correctly\n");
// }



void dummyfree(void* pItem)
{
    //do nothing
}

//stress test allocation
//assumes LIST_MAX_NUM_NODES is a multiple of LIST_MAX_NUM_HEADS
void test0()
{
    printf("test0: list and node allocation/deallocation\n");
    List* lists[LIST_MAX_NUM_HEADS];
    //fully allocate and deallocate all heads and nodes 100 times
    int val = 1;
    for(int i = 0; i< 100; i++){
        for(int j = 0; j < LIST_MAX_NUM_HEADS; j++)
        {
            lists[j] = List_create();
            assert(lists[j] != NULL);
            for(int k = 0; k < LIST_MAX_NUM_NODES / LIST_MAX_NUM_HEADS; k++)
            {
                assert(List_add(lists[j], &val) == 0);
            }
        }
        assert(List_create() == NULL);
        assert(List_add(lists[0], &val) == -1);

        for(int n = 0; n < LIST_MAX_NUM_HEADS; n++)
        {
            List_free(lists[n], &dummyfree);
        }
    }
    //fully allocate all nodes and heads 100 times
    printf("test0 passed\n");
}

//test List_create(), add, insert
void test1()
{
    printf("test1: test List_create(), list_add(), List_insert()\n");
    int nums[4] = {0,1,2,3};
    List* test_list = List_create();
    List_add(test_list, &nums[0]);
    List_insert(test_list, &nums[1]);
    List_add(test_list, &nums[2]);
    List_insert(test_list, &nums[3]);
    //validate (first: 1, last: 0, current: 3, order: 1,3,2,0)
    void* control_list[7] = {&nums[1],&nums[0],&nums[3],&nums[1],&nums[3],&nums[2],&nums[0]};
    validateList(test_list,control_list);
    printf("test1 passed\n");
}

//add three elements to list. 
//move to first element, validate
//move before first, validate
//move to end, validate
//move after end, validate
void test2()
{
    printf("test2: test List_next(), List_prev(), List_first(), List_last()\n");
    int nums[3] = {0,1,2};
    List* test_list = List_create();
    List_add(test_list, &nums[0]);
    List_add(test_list, &nums[1]);
    List_add(test_list, &nums[2]);
    List_prev(test_list);
    List_prev(test_list);
    //validate (first: 0, last: 2, current: 0, order: 0,1,2)
    void* control_list[7] = {&nums[0],&nums[2],&nums[0],&nums[0],&nums[1],&nums[2]};
    validateList(test_list, control_list);
    List_prev(test_list);
    setControlList(control_list,&nums[0],&nums[2],NULL);
    validateList(test_list, control_list);
    List_next(test_list);
    List_next(test_list);
    List_next(test_list);
    setControlList(control_list,&nums[0],&nums[2],&nums[2]);
    validateList(test_list, control_list);
    List_next(test_list);
    setControlList(control_list,&nums[0],&nums[2],NULL);
    validateList(test_list, control_list);
    printf("test2 passed\n");
}

//alternate appending and prepending value
void test3()
{
    printf("test3: test list_prepend(), list_append()\n");
    int nums[4] = {0,1,2,3};
    List* test_list = List_create();
    List_append(test_list, &nums[0]);
    List_prepend(test_list, &nums[1]);
    List_append(test_list, &nums[2]);
    List_prepend(test_list, &nums[3]);
    //validate (first: 3, last: 2, current: 3, order: 3,1,0,2)
    void* control_list[7] = {&nums[3],&nums[2],&nums[3],&nums[3],&nums[1],&nums[0],&nums[2]};
    validateList(test_list,control_list);
    printf("test3 passed\n");
}

//test deletes and trims
void test4()
{
    printf("test4: list_delete(), list_trim()\n");
    int nums[6] = {0,1,2,3,4,5};
    List* test_list = List_create();
    List_add(test_list, &nums[0]);
    List_add(test_list, &nums[1]);
    List_add(test_list, &nums[2]);
    List_add(test_list, &nums[3]);
    List_add(test_list, &nums[4]);
    //remove last
    assert(List_remove(test_list) == &nums[4]);
    //validate (first: 0, last: 3, current: NULL, order: 0,1,2,3)
    void* control_list_a[7] = {&nums[0],&nums[3],NULL,&nums[0],&nums[1],&nums[2],&nums[3]};
    validateList(test_list,control_list_a);
    //remove first
    List_first(test_list);
    assert(List_remove(test_list) == &nums[0]);
    //validate (first: 1, last: 3, current: NULL, order: 1,2,3)
    void* control_list_b[6] = {&nums[1],&nums[3],&nums[1],&nums[1],&nums[2],&nums[3]};
    validateList(test_list,control_list_b);
    //trim
    assert(List_trim(test_list) == &nums[3]);
    //validate (first: 1, last: 2, current: 2, order: 1,2)
    void* control_list_c[5] = {&nums[1],&nums[2],&nums[2],&nums[1],&nums[2]};
    validateList(test_list,control_list_c);
    //delete last two
    List_first(test_list);
    assert(List_remove(test_list) == &nums[1]);
    List_first(test_list);
    assert(List_remove(test_list) == &nums[2]);
    void* control_list_d[3] = {NULL,NULL,NULL};
    validateList(test_list,control_list_d);
    printf("test4 passed\n");
}

//test concatinate
void test5()
{
    printf("test5: list_concat()\n");
    List* test_list_1 = List_create();
    List* test_list_2 = List_create();
    List* test_list_3 = List_create();
    List* test_list_4 = List_create();
    //concat two lists with elements
    int nums[4] = {0,1,2,3};
    List_add(test_list_1, &nums[0]);
    List_add(test_list_1, &nums[1]);
    List_add(test_list_2, &nums[2]);
    List_add(test_list_2, &nums[3]);
    List_concat(test_list_1,test_list_2);
    void* control_list_a[7] = {&nums[0],&nums[3],&nums[1],&nums[0],&nums[1],&nums[2],&nums[3]};
    validateList(test_list_1,control_list_a);
    //concat empty with full
    List_concat(test_list_3,test_list_1);
    void* control_list_b[7] = {&nums[0],&nums[3],NULL,&nums[0],&nums[1],&nums[2],&nums[3]};
    validateList(test_list_3,control_list_b);
    //concat full with empty
    List_concat(test_list_3,test_list_4);
    void* control_list_c[7] = {&nums[0],&nums[3],NULL,&nums[0],&nums[1],&nums[2],&nums[3]};
    validateList(test_list_3,control_list_c);
    printf("test5 passed\n");
}


static int test6_counter;
void Test6_Print(void* pItem)
{
    test6_counter += *(int*)pItem;
}

//test free
void test6()
{
    printf("test6: List_free()\n");
    List* test_list = List_create();
    int nums[4] = {0,1,2,3};
    List_add(test_list, &nums[0]);
    List_add(test_list, &nums[1]);
    List_add(test_list, &nums[2]);
    List_add(test_list, &nums[3]);
    test6_counter = 0;
    List_free(test_list, &Test6_Print);
    assert(test6_counter == 6);
    printf("test6 passed\n");
}

bool test7_comp(void* pItem, void* pComparisonArg)
{
    return *(int*)pItem == *(int*)pComparisonArg;
}

//test search
void test7()
{
    printf("test7: List_search()\n");
    List* test_list = List_create();
    int nums[4] = {0,1,2,3};
    //search empty list
    assert(List_search(test_list, &test7_comp, &nums[0]) == NULL);
    List_add(test_list, &nums[0]);
    List_add(test_list, &nums[1]);
    List_add(test_list, &nums[2]);
    List_add(test_list, &nums[3]);
    //search from first
    List_first(test_list);
    assert(List_search(test_list, &test7_comp, &nums[2]) == &nums[2]);
    assert(List_curr(test_list) == &nums[2]);
    //search from after last
    List_last(test_list);
    List_next(test_list);
    assert(List_search(test_list, &test7_comp, &nums[2]) == NULL);
    assert(List_curr(test_list) == NULL);
    //search from before first
    List_first(test_list);
    List_prev(test_list);
    assert(List_search(test_list, &test7_comp, &nums[2]) == &nums[2]);
    assert(List_curr(test_list) == &nums[2]);
    printf("test7 passed\n");
}

int main()
{
    InitNodeIDs();
    test0();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
}

