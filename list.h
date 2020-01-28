

LIST *ListCreate();

int ListCount(list);

void *ListFirst(list); 

void *ListLast(list);

void *ListNext(list);

void *ListPrev(list);

void *ListCurr(list);

int ListAdd(list, item);

int ListInsert(list, item);

int ListAppend(list, item);

int ListPrepend(list, item);

void *ListRemove(list);

void ListConcat(list1, list2);

void ListFree(list, itemFree);

void *ListTrim(list);

void *ListSearch(list, comparator, comparisonArg);
