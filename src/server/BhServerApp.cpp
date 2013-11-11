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
		    m_sockBufPunmap.erase(nSock);
		    close(nSock);
		}
		break;
	    }
	    else
	    {
		m_sockBufPunmap.erase(nSock);
		close(nSock);
		break;
	    }
	}
	else if (0 == nReadLen)
	{
	    m_sockBufPunmap.erase(nSock);
	    close(nSock);
	    break;
	}
	else
	{
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
		break;
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
			    m_sockBufPunmap[nSock] = new BhMemeryPool(m_nBlockLength);
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
		    m_sockBufUnmap.erase(events[i].data.fd);
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
}
