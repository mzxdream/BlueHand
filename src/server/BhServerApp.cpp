#include "BhServerApp.h"

BhServerApp& BhServerApp::Instance()
{
    static BhServerApp instance;
    return instance;
}
bool BhServerApp::Init(int nPort, int nListenCount, int nHandleCount)
{
    m_nPort = nPort;
    m_nListenCount = nListenCount;
    m_nHandleCount = nHandleCount;
    return true;
}
void BhServerApp::Clear()
{
    std::list<int>::iterator iter = m_sockList.begin();
    for (;iter != m_sockList.end(); ++iter)
    {
	close(*iter);
    }
}
void BhServerApp::Run()
{
    int nLisSock = 0;
    if ((nLisSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
	return;
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
}
