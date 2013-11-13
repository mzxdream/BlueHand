#include "BhServerApp.h"
#include "../kernal/BhSocket.h"
#include "../kernal/BhThreadPool.h"
#include <sys/epoll.h>
#include <errno.h>

BhServerApp::BhServerApp()
    : m_pThread(nullptr)
    , m_bWantRun(true)
{
}
BhServerApp::~BhServerApp()
{
    Clear();
}
BhServerApp& BhServerApp::Instance()
{
    static BhServerApp instance;
    return instance;
}
bool BhServerApp::Init(const std::string &strIP, int nPort, int nListenCount, int nHandleCount
		       , int nBLockLength, int nEpollTimeOut, int nMsgHeaderLen)
{
    m_bWantRun = true;
    m_strIP = strIP;
    m_nPort = nPort;
    m_nListenCount = nListenCount;
    m_nHandleCount = nHandleCount;
    m_nBlockLength = nBlockLength;
    m_nEpollTimeOut = nEpollTimeOut;
    m_nMsgHeaderLen = nMsgHeaderLen;
    return true;
}
void BhServerApp::Clear()
{
    delete m_pThread;
    m_pThread = nullptr;
}
bool BhServerApp::Start()
{
    m_bWantRun = true;
    m_pThread = new boost::thread(boost::bind(&BhServerApp::Run, this));
}
void BhServerApp::Stop()
{
    m_bWantRun = false;
}
void BhServerApp::Wait()
{
    m_pThread->join();
}

void BhServerApp::HandleMsg(int nEpoll, int nSock)
{
    char *pBuf = new char[m_nBlockLength];
    int nReadLen = 0;
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    int nMsgLen = 0;
    int i = 0;
    boost::ptr_unordered_map<int, BhMemeryPool>::iterator iter;
   
    while (m_bWantRun)
    {
	nReadLen = recv(nSock, pBuf, m_nBlockLength, 0);
	if (nReadLen < 0)//出错
	{
	    if (errno == EAGAIN
		|| errno == EWOULDBLOCK
		|| errno == EINTER)
	    {
		event.data.fd = nSock;
		if (-1 == (epoll_ctl(efd, EPOLL_CTL_ADD, nSock, &event)))
		{
		    {
			WriteLock lock(m_sockInfoMutex);
			m_sockInfoPunmap.erase(nSock);
		    }
		    {
			WriteLock lock(m_sockBufMutex);
			m_sockBufMutex.erase(nSock);
		    }
		    close(nSock);
		}
		break;
	    }
	    else
	    {
		{
		    WriteLock lock(m_sockBufMutex);
		    m_sockBufPunmap.erase(nSock);
		}
		{
		    WriteLock lock(m_sockInfoMutex);
		    m_sockInfoPunmap.erase(nSock);
		}
		close(nSock);
		break;
	    }
	}
	else if (0 == nReadLen)
	{
	    {
		WriteLock lock(m_sockBufMutex);
		m_sockBufPunmap.erase(nSock);
	    }
	    {
		WriteLock lock(m_sockInfoMutex);
		m_sockInfoPunmap.erase(nSock);
	    }
	    close(nSock);
	    break;
	}
	else
	{
	    ReadLock lock(m_sockBufMutex);
	    iter = m_sockBufPunmap.find(nSock);
	    if (iter == m_sockBufPunmap.end())
	    {
		close(nSock);
		break;
	    }
	    iter->Write(pBuf, nReadLen);
	    while (m_bWantRun)
	    {
		if (iter->Length() > m_nMsgHeaderLen)//判断能否凑成一条完整的消息
		{
		    if (iter->Read(pBuf, m_nMsgHeaderLen))
		    {
			nMsgLen = pMsgLen[0];
			for (i = 1; i < m_nMsgHeaderLen; ++i)
			{
			    nMsgLen <<= 8;
			    nMsgLen += pMsgLen[i];
			}
			if (iter->Length() >= nMsgLen + m_nMsgHeaderLen)
			{
			    if (iter->Read(pBuf, nMsgLen + m_nMsgHeaderLen))
			    {
				//处理信息
				IMsg *pMsg = Msg::UnPack(pBuf + m_nMsgHeaderLen, nMsgLen);
				if (pMsg)
				{
				    MsgHandler::Instance().Invoke(pMsg);
				    delete pMsg;
				}
				//释放空间
				iter->Free(nMsgLen + m_nMsgHeaderLen);
				continue;
			    }
			}
		    }
		}
		else
		{
		    break;
		}
	    }
	}
    }
    delete[] pBuf;
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
    
    while (m_bWantRun)
    {
	if ((nEventCount = epoll_wait(efd, events, m_nListenCount +1, m_nEpollTimeOut)) < 0)
	{
	    break;
	}
	for (i = 0; i < nEventCount; ++i)
	{
	    if (nLisSock == events[i].data.fd)
	    {
		while (m_bWantRun)
		{
		    nSockLen = sizeof(struct sockaddr);
		    nSock = accept(nLisSock, &addr, &nSockLen);
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
			    {
				WriteLock lock(m_sockBufMutex);
				m_sockBufPunmap.insert(nSock, new BhMemeryPool(m_nBlockLength));
			    }
			    {
				std::string strIP = inet_ntoa(addr.sin_addr);
				int nPort = ntohs(addr.sin_port);
				WriteLock lock(m_sockInfoMutex);
				m_sockInfoPunmap.insert(nSock, new BhSockInfo(nSock, strIP, nPort));
			    }
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
			WriteLock lock(m_sockBufMutex);
			m_sockBufUnmap.erase(events[i].data.fd);
		    }
		    {
			WriteLock lock(m_sockInfoMutex);
			m_sockInfoMutex.erase(events[i].data.fd);
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
    boost::ptr_unordered_map<int, BhMemeryPool>::iterator iter = m_sockBufPunmap.begin();
    while (iter != m_sockBufPunmap.end())
    {
	
    }
}
