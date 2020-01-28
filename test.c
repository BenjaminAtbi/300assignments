
#include "list.h"
#include <stdio.h>

int main()
{

	printf("suh\n");

	struct LIST *l = ListCreate();
	printf("this-> %i\n", l->length);	
	return 0;
}

