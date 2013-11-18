#include "BhServerApp.h"
#include "../kernal/BhSocket.h"
#include "../kernal/BhThreadPool.h"
#include "../kernal/MsgPacket.h"
#include "../kernal/MsgHandler.h"
#include "../msg/INetMsg.h"
#include <sys/epoll.h>
#include <errno.h>

BhServerApp::BhServerApp()
    : m_bWantRun(true)
    , m_pThread(nullptr)

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
		       , unsigned uBlockLen, int nEpollTimeOut, int nMsgHeaderLen)
{
    m_bWantRun = true;
    m_strIP = strIP;
    m_nPort = nPort;
    m_nListenCount = nListenCount;
    m_nHandleCount = nHandleCount;
    m_uBlockLen = uBlockLen;
    m_nEpollTimeOut = nEpollTimeOut;
    m_nMsgHeaderLen = nMsgHeaderLen;
    return true;
}
void BhServerApp::Clear()
{
    if (m_pThread)
    {
	delete m_pThread;
	m_pThread = nullptr;
    }
}
bool BhServerApp::Start()
{
    m_bWantRun = true;
    m_pThread = new boost::thread(boost::bind(&BhServerApp::Run, this));
    return nullptr != m_pThread;
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
    char *pBuf = new char[m_uBlockLen];
    int nReadLen = 0;
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    int nMsgLen = 0;
    int i = 0;
    
    boost::ptr_unordered_map<int, BhSockInfo>::iterator iter;
   
    while (m_bWantRun)
    {
	nReadLen = recv(nSock, pBuf, m_uBlockLen, 0);
	if (nReadLen < 0)//出错
	{
	    if (errno == EAGAIN
		|| errno == EWOULDBLOCK
		|| errno == EINTR)
	    {
		event.data.fd = nSock;
		if (-1 == (epoll_ctl(nEpoll, EPOLL_CTL_ADD, nSock, &event)))
		{
		    {
			WriteLock lock(m_sockInfoMutex);
			m_sockInfoPunmap.erase(nSock);
		    }
		    close(nSock);
		}
		break;
	    }
	    else
	    {
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
		WriteLock lock(m_sockInfoMutex);
		m_sockInfoPunmap.erase(nSock);
	    }
	    close(nSock);
	    break;
	}
	else
	{
	    ReadLock lock(m_sockInfoMutex);
	    iter->second->Pool().Write(pBuf, nReadLen);
	    while (m_bWantRun)
	    {
		if (iter->second->Pool().Length() > static_cast<unsigned>(m_nMsgHeaderLen))//判断能否凑成一条完整的消息
		{
		    if (iter->second->Pool().Read(pBuf, m_nMsgHeaderLen))
		    {
			nMsgLen = pBuf[0];
			for (i = 1; i < m_nMsgHeaderLen; ++i)
			{
			    nMsgLen <<= 8;
			    nMsgLen += pBuf[i];
			}
			if (iter->second->Pool().Length() >= static_cast<unsigned>(nMsgLen + m_nMsgHeaderLen))
			{
			    if (iter->second->Pool().Read(pBuf, nMsgLen + m_nMsgHeaderLen))
			    {
				//处理信息
				IMsg *pMsg = MsgPacket::UnPack(pBuf + m_nMsgHeaderLen, nMsgLen);
				if (pMsg)
				{
				    INetMsg *pNetMsg = dynamic_cast<INetMsg *>(pMsg);
				    if (pNetMsg)
				    {
					pNetMsg->NetID(iter->second->Sock());
					MsgHandler::Instance().Invoke(pMsg);
				    }
				    delete pMsg;
				}
				//释放空间
				iter->second->Pool().Free(nMsgLen + m_nMsgHeaderLen);
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
    struct epoll_event event;
    struct epoll_event events[m_nListenCount + 1];
    int nLisSock = listenSock.Detach();
    event.data.fd = nLisSock;
    event.events = EPOLLIN | EPOLLET;
    if (-1 == epoll_ctl(efd, EPOLL_CTL_ADD, nLisSock, &event))
    {
	return;
    }
    int nSock = 0;
    int i = 0;
    int nEventCount = 0;
    struct sockaddr_in addr;
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
