#include "BhSockInfo.h"

BhSockInfo::BhSockInfo(int nSock, const std::string &strIP, int nPort, unsigned uBlockLen)
    : m_nSock(nSock)
    , m_strIP(strIP)
    , m_nPort(nPort)
    , m_pool(uBlockLen)
{
}
BhSockInfo::BhSockInfo(unsigned uBlockLen)
    : m_pool(uBlockLen)
{
}
int BhSockInfo::Sock() const
{
    return m_nSock;
}
void BhSockInfo::Sock(int nSock)
{
    m_nSock = nSock;
}
const std::string& BhSockInfo::IP() const
{
    return m_strIP;
}
void BhSockInfo::IP(const std::string &strIP)
{
    m_strIP = strIP;
}
int BhSockInfo::Port() const
{
    return m_nPort;
}
void BhSockInfo::Port(int nPort)
{
    m_nPort = nPort;
}
BhMemeryPool& BhSockInfo::Pool()
{
    return m_pool;
}
