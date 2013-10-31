#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "BhThreadPool.h"
#include <iostream>

#define SERV_PORT 3230
#define LISTEN_MAX 100
#define EPOLL_MAX 100
typedef tagNetMsgHeader
{
    char cLen[2];
}NetMsgHeader;
bool SetSockNonBlock(int nSock)
{
    int nFlags = 0;
    if ((nFlags = fcntl(nSock, F_GETFL, 0)) != -1)
    {
	if (fcntl(nSock, F_SETFL, nFlags | O_NONBLOCK) != -1)
	{
	    return true;
	}
    }
    return false;
}
int RecvAll(int nSock, void *buf, int nLen)
{
    int nRecvLen = 0;
    ssize_t sockRet = 0;
    while (true)
    {
	sockRet = recv(nSock, buf+nRecvLen, nLen - nRecvLen);
	if (sockRet < 0)
	{
	    if (0 == nRecvLen && errno == EAGAIN)
	    {
		return 0;
	    }
	    else if (errno == EAGAIN
		|| errno == EWOULDBLOCK
		|| errno == EINTR)
	    {
		continue;
	    }
	    else
	    {
		return -1;
	    }
	}
	else if (0 == sockRet)
	{
	    return -1;
	}
	else
	{
	    nRecvlen += sockRet;
	    if (nRecvLen == nLen)
	    {
		return 1;
	    }
	}
    }
    return -1;
}
void HandleMsg(int nSock, int nEpoll)
{
    NetMsgHeader header;
    int nRet = 0;
    while (true)
    {
	//读取消息头
	nRet = RecvAll(nSock, &header, sizeof(header));
	if (-1 == nRet)
	{
	    close(nSock);
	    return;
	}
	else if (0 == nRet)
	{
	    struct epoll_event event;
	    event.data.fd = nInSock;
	    event.events = EPOLLIN | EPOLLET;
	    if (-1 == (epoll_ctl(efd, EPOLL_CTL_ADD, nInSock, &event)))
	    {
		return;
	    }
	    else
	    {
		return;
	    }
	}
	else if (1 == nRet)
	{
	    int nLen = header.cLen[0] << 8 + header.cLen[1];
	    char *buf = new char[nLen + 1];
	    nRet = RecvAll(nSock, buf, nLen);
	    if (nRet != 1)
	    {
		close(nSock);
		return;
	    }
	    buf[nLen + 1] = '\0';
	    //调用msghandle
	}
    }
}
int main(int argc, char *argv[])
{
    int nLisSock = 0;
    if ((nLisSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
	return 0;
    }
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERV_PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(nLisSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
	return 0;
    }
    if (!SetSockNonBlock(nLisSock))
    {
	return 0;
    }
    if (listen(nLisSock, LISTEN_MAX) < 0)
    {
	return 0;
    }
    struct epoll_event event;
    struct epoll_event events[EPOLL_MAX];
    int efd = 0;
    if (-1 == (efd = epoll_create1(0)))
    {
	return 0;
    }
    event.data.fd = nLisSock;
    event.events = EPOLLIN | EPOLLET;
    if (-1 == epoll_ctl(efd, EPOLL_CTL_ADD, nLisSock, &event))
    {
	return 0;
    }
    
    int nEventCount = 0;
    int i = 0;
    struct sockaddr inAddr;
    socklen_t inLen = sizeof(struct sockaddr);
    int nInSock = 0;
    BhThreadPool pool(3);
    pool.PushTask(boost::bind(HandMsg, 10, 10));
    while (true)
    {
	if ((nEventCount = epoll_wait(efd, events, EPOLL_MAX, -1)) < 0)
	{
	    break;
	}
	for (i = 0; i < nEventCount; ++i)
	{
	    if ((events[i].events & EPOLLERR)
		|| (events[i].events & EPOLLHUP)
		|| (events[i].events & EPOLLIN))
	    {
		close(events[i].data.fd);
	    }
	    else if (nLisSock == events[i].data.fd)
	    {
		while (true)
		{
		    inLen = sizeof(struct sockaddr);
		    if (-1 == (nInSock = accept(nInSock, &inAddr, &inLen)))
		    {
			break;
		    }
		    if (!SetSockNonBlock(nInSock))
		    {
			abort();
		    }
		    event.data.fd = nInSock;
		    event.events = EPOLLIN | EPOLLET;
		    if (-1 == (epoll_ctl(efd, EPOLL_CTL_ADD, nInSock, &event)))
		    {
			return 0;
		    }
		}
	    }
	    else
	    {
		pool.PushTask(boost::bind(HandleMsg, events[i].data.fd, efd));
	    }
	}
    }
    return 0;
}
