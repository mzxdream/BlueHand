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
#define EPOLL_LEN 100

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
    struct epoll_event event;
    struct epoll_event events[EPOLL_LEN];
    int efd = 0;
    if ((efd = epoll_create1(0)) == -1)
    {
	return 0;
    }
    event.data.fd = nSock;
    event.events = EPOLLIN | EPOLLET;
    if (-1 == epoll_ctl(efd, EPOLL_CTL_ADD, nSock, &event))
    {
	return 0;
    }
    while (true)
    {
	int n = 0;
	n = epoll_wait(efd, events, EPOLL_LEN, -1);
	for (int i = 0; i < n; ++i)
	{
	    if ((events[i].events & EPOLLERR)
		|| (events[i].events & EPOLLHUP)
		|| !(events[i].events & EPOLLIN))
	    {
		close(events[i].data.fd);
	    }
	    else if (nSock == events[i].data.fd)
	    {
		while (true)
		{
		    struct sockaddr inAddr;
		    socklen_t inLen = sizeof (struct sockaddr);
		    int nInfd;
		    if (-1 == (nInfd = accept(nSock, &inAddr, &inLen)))
		    {
			break;
		    }
		    if (!SetNonBlock(nInfd))
		    {
			return 0;
		    }
		    event.data.fd = nInfd;
		    event.events = EPOLLIN | EPOLLET;
		    if (-1 == (epoll_ctl(efd, EPOLL_CTL_ADD, nInfd, &event)))
		    {
			return 0;
		    }
		}
	    }
	    else
	    {
		int done = 0;
		while (true)
		{
		    ssize_t count;
		    char buf[512];
		    count = 
		}
	    }
	}
    }
    return 0;
    
}
