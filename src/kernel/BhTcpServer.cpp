#include "BhTcpServer.h"
#include "BhThreadPool.h"
#include <sys/epoll.h>
#include <error.h>

BhTcpServer::BhTcpServer(const InvokeFunc &func)
    : m_pThread(nullptr)
    , m_invokeFunc(func)
{
}
BhTcpServer::BhTcpServer()
    : m_pThread(nullptr)
{
}
BhTcpServer::~BhTcpServer()
{
    Clear();
}
void BhTcpServer::SetFunc(InvokeFunc func)
{
    m_invokeFunc = func;
}
bool BhTcpServer::Init(const std::string &strIP, int nPort, int nListenCount, int nEpollCount, int nEpollTimeOut, int nThreadCount, unsigned uBlockLen)
{
    m_strIP = strIP;
    m_nPort = nPort;
    m_nListenCount = nListenCount;
    m_nEpollCount = nEpollCount;
    m_nEpollTimeOut = nEpollTimeOut;
    m_nThreadCount = nThreadCount;
    m_uBlockLen = uBlockLen;
    return true;
}
void BhTcpServer::Clear()
{
    if (m_pThread)
    {
	delete m_pThread;
	m_pThread = nullptr;
    }
}
void BhTcpServer::CloseSock(int nSock)
{
    WriteLock lock(m_sockInfoMutex);
    boost::ptr_list<BhSockInfo>::iterator iter = m_sockInfoPlist.begin();
    for (; iter != m_sockInfoPlist.end(); ++iter)
    {
	if (iter->Sock() == nSock)
	{
	    m_sockInfoPlist.erase(iter);
	    close(nSock);
	    break;
	}
    }
}
void BhTcpServer::HandleMsg(int nEpoll, int nSock)
{
    char *pBuf = new char[m_uBlockLen];
    int nReadLen = 0;
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    
    while (m_bWantRun)
    {
	nReadLen = recv(nSock, pBuf, m_uBlockLen, 0);
	if (nReadLen < 0)//出错
	{
	    event.data.fd = nSock;
	    if ((errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)//如果当前错误码不是没有可写数据或者不能加入epoll
		|| -1 == (epoll_ctl(nEpoll, EPOLL_CTL_ADD, nSock, &event)))
	    {
		CloseSock(nSock);
	    }
	    break;
	}
	else if (0 == nReadLen)//对面关闭sock
	{
	    CloseSock(nSock);
	    break;
	}
	else
	{
	    ReadLock lock(m_sockInfoMutex);
	    boost::ptr_list<BhSockInfo>::iterator iter = m_sockInfoPlist.begin();
	    for (; iter != m_sockInfoPlist.end(); ++iter)
	    {
		if (iter->Sock() == nSock)
		{
		    iter->Pool().Write(pBuf, nReadLen);
		    while (m_bWantRun)
		    {
			if (!m_invokeFunc(nSock, iter->Pool()))
			{
			    break;
			}
		    }
		    break;
		}
	    }
	}
    }
    delete[] pBuf;
}
bool BhTcpServer::Start()
{
    m_bWantRun = true;
    m_pThread = new boost::thread(boost::bind(&BhTcpServer::Run, this));
    return nullptr != m_pThread;
}
void BhTcpServer::Stop()
{
    m_bWantRun = false;
}
void BhTcpServer::Wait()
{
    if (m_pThread)
    {
	m_pThread->join();
    }
}

void BhTcpServer::Run()
{
    BhSocket listenSock;
    if (!listenSock.Create(SockType::TCP)
	|| !listenSock.Bind(m_nPort, m_strIP)
	|| !listenSock.SetNonBlock()
	|| !listenSock.Listen(m_nListenCount)
	)
    {
	return;
    }
    int nEpoll = -1;
    if (-1 == (nEpoll = epoll_create1(0)))
    {
	return;
    }
    int nListenSock = listenSock.Detach();
    struct epoll_event event;
    struct epoll_event events[m_nEpollCount + 1];
    event.data.fd = nEpoll;
    event.events = EPOLLIN | EPOLLET;
    if (-1 == epoll_ctl(nEpoll, EPOLL_CTL_ADD, nListenSock, &event))
    {
	return;
    }
    int nSock = 0;
    int nEventCount = 0;
    int i = 0;
    struct sockaddr_in addr;
    socklen_t sockLen = sizeof(struct sockaddr);
    BhThreadPool pool(m_nThreadCount);
    
    while (m_bWantRun)
    {
	if ((nEventCount = epoll_wait(nEpoll, events, m_nEpollCount + 1, m_nEpollTimeOut)) < 0)
	{
	    break;
	}
	for (i = 0; i < nEventCount; ++i)
	{
	    if (nListenSock == events[i].data.fd)
	    {
		while (m_bWantRun)
		{
		    nSock = accept(nListenSock, reinterpret_cast<struct sockaddr *>(&addr), &sockLen);
		    if (-1 == nSock)
		    {
			if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
			{
			    m_bWantRun = false;
			    break;
			}
		    }
		    else
		    {
			event.data.fd = nSock;
			event.events = EPOLLIN | EPOLLET;
			if (BhSocket::SetNonBlock(nSock) && epoll_ctl(nEpoll, EPOLL_CTL_ADD, nSock, &event) != -1)
			{
			    std::string strIP = inet_ntoa(addr.sin_addr);
			    int nPort = ntohs(addr.sin_port);
			    WriteLock lock(m_sockInfoMutex);
			    m_sockInfoPlist.push_back(new BhSockInfo(nSock, strIP, nPort, m_uBlockLen));
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
		if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || !(events[i].events & EPOLLIN))
		{
		    CloseSock(nSock);
		}
		else
		{
		    epoll_ctl(nEpoll, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
		    pool.PushTask(boost::bind(&BhTcpServer::HandleMsg, this, nEpoll, events[i].data.fd));
		}
	    }
	}
    }
    close(nEpoll);
    pool.Interrupt();
    pool.Join();
    boost::ptr_list<BhSockInfo>::iterator iter = m_sockInfoPlist.begin();
    while (iter != m_sockInfoPlist.end())
    {
	close(iter->Sock());
	iter = m_sockInfoPlist.erase(iter);
    }
}
