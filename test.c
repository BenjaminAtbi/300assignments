#include <stdio.h>

void testExit(char* err)
{
	printf(err);
}

int main()
{
	testExit("shit");
}
