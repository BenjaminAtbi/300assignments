#include "client.h"
#include <stdio.h>


int i = 0;

void *threadoutput(void* buf)
{
    i++;
    printf("num: %i: %s\n",i, (char *) buf);
}

int threadtest()
{
    pthread_t th1, th2, th3, th4;
    int ret1, ret2, ret3, ret4;
    ret1 = pthread_create( &th1, NULL, threadoutput, (void*) "u mum gay");
    ret2 = pthread_create( &th2, NULL, threadoutput, (void*) "no u");
    ret3 = pthread_create( &th3, NULL, threadoutput, (void*) "omegalul");
    ret4 = pthread_create( &th4, NULL, threadoutput, (void*) "fuck mcdonalds");
    printf("%i, %i, %i, %i\n", ret1, ret2, ret3, ret4);

    sleep(3);
}

int main(int argc, char *argv[])
{
    printf("testing threads \n");
    threadtest();
}
