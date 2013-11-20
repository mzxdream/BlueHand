#include "BhServerApp.h"
#include "../kernel/MsgPacket.h"

BhServerApp::BhServerApp()
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
bool BhServerApp::Init(const std::string &strIP, int nPort, int nListenCount, int nEpollCount, int nEpollTimeOut, int nThreadCount, unsigned uBlockLen)
{
    if (!m_tcpServer.Init(strIP, nPort, nListenCount, nEpollCount, nEpollTimeOut, nThreadCount, uBlockLen))
    {
	return false;
    }
    m_tcpServer.SetFunc(boost::bind(&BhServerApp::HandleSockBuf, this, _1, _2));
    return true;
}
void BhServerApp::Clear()
{
    m_tcpServer.Clear();
}
bool BhServerApp::Start()
{
    if (!m_tcpServer.Start())
    {
	return false;
    }
    return true;
}
void BhServerApp::Stop()
{
    m_tcpServer.Stop();
}
void BhServerApp::Wait()
{
    m_tcpServer.Wait();
}
bool BhServerApp::HandleSockBuf(int nSock, BhMemeryPool &pool)
{
    int nMsgLen = 0;
    if (pool.Read(reinterpret_cast<char *>(&nMsgLen), sizeof(int)))
    {
	char pBuf = new char[sizeof(int) + nMsgLen];
	if (pool.Read(pBuf, nMsgLen + sizeof(int)))
	{
	    IMsg *pMsg = MsgPacket::UnPack(pBuf + sizeof(int), nMsgLen);
	    if (pMsg)
	    {
		INetMsg *pNetMsg = dynamic_cast<INetMsg *>(pMsg);
		if (pNetMsg)
		{
		}
	    }
	}
	delete pBuf;
    }
    return false;
}
