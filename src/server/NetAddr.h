#ifndef _NETADDR_H_
#define _NETADDR_H_

#include <string>

class NetAddr
{
public:
    NetAddr() = default;
    ~NetAddr() = default;
    NetAddr(const NetAddr &) = default;
    NetAddr& operator=(const NetAddr &) = default;
public:
    const std::string& IP() const;
    void IP(const std::string &strIP);
    int Port() const;
    void Port(int nPort);
private:
    std::string m_strIP;
    int m_nPort;
};

#endif
