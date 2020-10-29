#include "client.h"


void reader(const params *addrs)
{
    
    while(1)
    {
        //create new message
        char* buf = malloc(MSGLENGTH * sizeof(char));
        memset(buf, 0, MSGLENGTH * sizeof(char));
        char c = '0';

        //accepts chars one at a time, until buffer is full or newline received
        //lines longer than MSGLENGTH are split into multiple lines
        for(int i = 0; c != '\n';)
        {
            c = getchar();
            buf[i] = c;

            if(i < MSGLENGTH-2)
            {
                i++;   
            } else {
                buf[MSGLENGTH-1] = '\n';
                break;
            }
        }
        //push message to send queue
        send_prepend(buf);

        //if user input exit message, exit
        if(buf[0] == '!')
        {
            exit_procedure(addrs);
        }
    }
}