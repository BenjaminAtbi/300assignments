#include "client.h"


void sender(addresses addrs)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM; //specify UDP
	hints.ai_flags = AI_PASSIVE; //specify address as local IP