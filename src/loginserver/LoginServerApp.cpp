#include "LoginServerApp.h"
#include "../kernel/NetMsgPacket.h"
#include "../kernel/MsgHandler.h"

LoginServerApp::LoginServerApp()
{
}
LoginServerApp::~LoginServerApp()
{
    Clear();
}
LoginServerApp& LoginServerApp::Instance()
{
    static LoginServerApp instance;
    return instance;
}
bool LoginServerApp::Init(const std::string &strIP, int nPort, int nListenCount, int nEpollCount, int nEpollTimeOut, int nThreadCount, unsigned uBlockLen)
{
    if (!m_tcpServer.Init(strIP, nPort, nListenCount, nEpollCount, nEpollTimeOut, nThreadCount, uBlockLen))
    {
	return false;
    }
    m_tcpServer.SetFunc(boost::bind(&LoginServerApp::HandleSockBuf, this, _1, _2));
    return true;
}
void LoginServerApp::Clear()
{
    m_tcpServer.Clear();
}
bool LoginServerApp::Start()
{
    if (!m_tcpServer.Start())
    {
	return false;
    }
    return true;
}
void LoginServerApp::Stop()
{
    m_tcpServer.Stop();
}
void LoginServerApp::Wait()
{
    m_tcpServer.Wait();
}
bool LoginServerApp::HandleSockBuf(int nSock, BhMemeryPool &pool)
{
    int nMsgLen = 0;
    if (pool.Read(reinterpret_cast<char *>(&nMsgLen), sizeof(int)))
    {
	char *pBuf = new char[sizeof(int) + nMsgLen];
	if (pool.Read(pBuf, nMsgLen + sizeof(int)))
	{
	    INetMsg *pMsg = NetMsgPacket::UnPack(pBuf + sizeof(int), nMsgLen);
	    if (pMsg)
	    {
		LoginMsg
		delete pMsg;
		return true;
	    }
	}
	delete pBuf;
    }
    return false;
}
