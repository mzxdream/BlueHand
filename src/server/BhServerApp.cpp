#include "BhServerApp.h"
#include "BhSocket.h"

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
}
void BhServerApp::Run()
{
    BhSocket listenSock;
    if (!listenSock.Create(SockType::TCP))
    {
	return;
    }
    if (!listenSock.Bind(m_nPort, m_strIP))
    {
	return;
    }
    if (!listenSock.SetNonBlock())
    {
	return;
    }
    if (!listenSock.Listen(m_nListenCount))
    {
	return;
    }
    int efd = 0;
    if (-1 == (efd = epoll_create1(0)))
    {
	return;
    }
    struct epoll_event event;
    struct epoll_event events[m_nListenCount + 1];
    int nLisSock = listenSock.Detach();
    event.data.fd = nLisSock;
    event.events = EPOLLIN | EPOLLET;
    if (-1 == epoll_ctl(efd, EPOLL_CTL_ADD, nLisSock, &event))
    {
	return 0;
    }
    int nSock = 0;
    int i = 0;
    int nEventCount = 0;
    struct sockaddr addr;
    socklen_t nSockLen;
    BhThreadPool pool(m_nHandleCount);
    
    pool.PushTask(boost::bind(&BhServerApp::HandMsg, this, 10, 10));
    while (m_bWantRun)
    {
	if ((nEventCount = epoll_wait(efd, events, m_nListenCount +1, -1)) < 0)
	{
	    break;
	}
	for (i = 0; i < nEventCount; ++i)
	{
	    if ((events[i].events & EPOLLERR)
		|| (events[i].events & EPOLLHUP)
		|| (events[i].events & EPOLLIN))
	    {
		m_sockBufUnmap.erase(events[i].data.fd);
		close(events[i].data.fd);
	    }
	    else if (nLisSock == events[i].data.fd)
	    {
		while (m_bWantRun)
		{
		    nSockLen = sizeof(struct sockaddr);
		    if ((nSock = accept(nLisSock, &addr, &nSockLen)) >= 0
			&& BhSocket::SetNonBlock(nSock))
		    {
			event.data.fd = nSock;
			event.events = EPOLLIN | EPOLLET;
			if (epoll_ctl(efd, EPOLL_CTL_ADD, nInSock, &event) >= 0)
			{
			    
			}
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
