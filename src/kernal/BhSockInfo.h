#ifndef _BHSOCKINFO_H_
#define _BHSOCKINFO_H_

#include <string>
#include "BhMemeryPool.h"

class BhSockInfo
{
public:
    explicit BhSockInfo(int nSock, const std::string &strIP, int nPort, unsigned uBlockLen);
    explicit BhSockInfo(unsigned uBlockLen);
    ~BhSockInfo() = default;
    BhSockInfo(const BhSockInfo &) = default;
    BhSockInfo& operator=(const BhSockInfo &) = default;
public:
    int Sock() const;
    void Sock(int nSock);
    const std::string& IP() const;
    void IP(const std::string &strIP);
    int Port() const;
    void Port(int nPort);
    BhMemeryPool& Pool();
private:
    int m_nSock;
    std::string m_strIP;
    int m_nPort;
    BhMemeryPool m_pool;
};

#endif
