#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SERVER_PORT 3230
#define LISTEN_LEN 100

bool SetNonBlock(int nSock)
{
    int nFlags = 0;
    int nTmp = 0;
    if ((nFlags = fcnt(nSock, F_GETFL, 0)) == -1)
    {
	return false;
    }
    nFlags |= O_NONBLOCK;
    if (fcntl(nSock, F_SETFL, nFlags) == -1)
    {
	return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    int nSock = 0;
    if ((nSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
	return 0;
    }
    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htnos(SERVER_PORT);
    servAddr.sin_addr.s_addr = htons(INADDR_ANY);
    if (bind(nSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
	return 0;
    }
    if (!SetNonBlock(nSock))
    {
	return 0;
    }
    if (listen(nSock, LISTEN_LEN) < 0)
    {
	return 0;
    }
    int efd = 0;
    if ((efd = epoll_create1(0)) == -1)
    {
	return 0;
    }
    struct epoll_event event;
    struct epoll_event *events;
    return 0;
    
}
