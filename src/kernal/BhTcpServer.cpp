#include "BhTcpServer.h"

BhTcpServer::BhTcpServer()
{
}
BhTcpServer::~BhTcpServer()
{
}
bool BhTcpServer::Init(const std::string &strIP, int nPort, int nListenCount, int nEpollCount, int nEpollTimeOut, int nThreadCount)
{
    m_strIP = strIP;
    m_nPort = nPort;
    m_nListenCount = nListenCount;
    m_nEpollCount = nEpollCount;
    m_nEpollTimeOut = nEpollTimeOut;
    m_nThreadCount = nThreadCount;
    BhSocket listenSock;
     if (!listenSock.Create(SockType::TCP))
    {
	return false;
    }
    if (!listenSock.Bind(m_nPort, m_strIP))
    {
	return false;
    }
    if (!listenSock.SetNonBlock())
    {
	return false;
    }
    if (!listenSock.Listen(m_nListenCount))
    {
	return false;
    }
    m_nListenSock = listenSock.Detach();
    if (-1 == (m_nEpoll = epoll_create1(0)))
    {
	return false;
    }
    return true;
}
void BhTcpServer::Run()
{
    struct epoll_event event;
    struct epoll_event events[m_nEpollCount + 1];
    event.data.fd = m_nEpoll;
    event.events = EPOLLIN | EPOLLET;
    if (-1 == epoll_ctl(m_nEpoll, EPOLL_CTL_ADD, m_nListenSock, &event))
    {
	return;
    }
    int nSock = 0;
    int i = 0;
    int nEventCount = 0;
    struct sockaddr_in addr;
    socklen_t nSockLen;
    BhThreadPool pool(m_nThreadCount);
    
    while (m_bWantRun)
    {
	if ((nEventCount = epoll_wait(m_nEpoll, events, m_nEpollCount, m_nEpollTimeOut)) < 0)
	{
	    break;
	}
	for (i = 0; i < nEventCount; ++i)
	{
	    if (m_nListenSock == events[i].data.fd)
	    {
		while (m_bWantRun)
		{
		    nSockLen = sizeof(struct sockaddr);
		    nSock = accept(nLisSock, reinterpret_cast<struct sockaddr *>(&addr), &nSockLen);
		    if (-1 == nSock)
		    {
			if ((errno != EAGAIN)
			    && (errno != EWOULDBLOCK))
			{
			    m_bWantRun = false;
			    break;
			}
		    }
		    else
		    {
			event.data.fd = nSock;
			event.events = EPOLLIN | EPOLLET;
			if (BhSocket::SetNonBlock(nSock)
			    && epoll_ctl(efd, EPOLL_CTL_ADD, nSock, &event) != -1)
			{
			    std::string strIP = inet_ntoa(addr.sin_addr);
			    int nPort = ntohs(addr.sin_port);
			    WriteLock lock(m_sockInfoMutex);
			    m_sockInfoPunmap.insert(nSock, new BhSockInfo(nSock, strIP, nPort, m_uBlockLen));
			}
			else
			{
			    close(nSock);			    
			}
		    }
		    
		}
	    }
	    else
	    {
		if ((events[i].events & EPOLLERR)
		    || (events[i].events & EPOLLHUP)
		    || (events[i].events & EPOLLIN))
		{
		    {
			WriteLock lock(m_sockInfoMutex);
			m_sockInfoPunmap.erase(events[i].data.fd);
		    }
		    close(events[i].data.fd);
		}
		else
		{
		    epoll_ctl(efd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
		    pool.PushTask(boost::bind(&BhServerApp::HandleMsg, this, efd, events[i].data.fd));
		}
	    }
	}
    }
    close(efd);
    pool.Interrupt();
    pool.Join();
    boost::ptr_unordered_map<int, BhSockInfo>::iterator iter = m_sockInfoPunmap.begin();
    while (iter != m_sockInfoPunmap.end())
    {
	close(iter->second->Sock());
	iter = m_sockInfoPunmap.erase(iter);
    }
}
