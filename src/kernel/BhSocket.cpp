#include "BhSocket.h"

BhSocket::BhSocket(int nSock)
    : m_nSock(nSock)
{
}
BhSocket::~BhSocket()
{
    Destory();
}
void BhSocket::Attach(int nSock)
{
    Destory();
    m_nSock = nSock;
}
int BhSocket::Detach()
{
    int nSock = m_nSock;
    m_nSock = -1;
    return nSock;
}
bool BhSocket::Create(SockType type)
{
    Destory();
    if (type == SockType::TCP)
    {
	m_nSock = socket(PF_INET, SOCK_STREAM, 0);
    }
    else if (type == SockType::UDP)
    {
	m_nSock = socket(PF_INET, SOCK_DGRAM, 0);
    }
    return m_nSock >= 0;
}
bool BhSocket::Bind(int nPort, const std::string &strIP)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(nPort);
    if (strIP.empty())
    {
	addr.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
	addr.sin_addr.s_addr = inet_addr(strIP.c_str());
    }
    return bind(m_nSock, (struct sockaddr *)&addr, sizeof(addr)) >= 0; 
}
bool BhSocket::Listen(int nCount)
{
    return listen(m_nSock, nCount) >= 0;
}
bool BhSocket::SetNonBlock()
{
    return BhSocket::SetNonBlock(m_nSock);
}
bool BhSocket::SetNonBlock(int nSock)
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
void BhSocket::Destory()
{
    if (m_nSock >= 0)
    {
	close(m_nSock);
    }
    m_nSock = -1;
}
