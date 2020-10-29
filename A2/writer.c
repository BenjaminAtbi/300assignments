#include "client.h"

void writer(params *addrs)
{
    while(1)
    {  
            char *buf = receive_trim();
            if(buf[0] == '!'){
                printf("\n####  Connection Closed ###\n");
                free(buf);
                pthread_exit(NULL);
            }
            printf("%.*s",MSGLENGTH, buf);
            free(buf);
    }
}