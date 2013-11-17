#ifndef _BHTCPSERVER_H_
#define _BHTCPSERVER_H_

#include "BhSocket.h"
#include <string>

class BhTcpServer
{
public:
    BhTcpServer();
    ~BhTcpServer();
    BhTcpServer& operator=(const BhTcpServer &) = delete;
    BhTcpServer(const BhTcpServer &) = delete;
public:
    bool Init();
    void Clear();
private:
    int m_nListenSock;
    int m_nEpoll;
    int m_nListenCount;
    int m_nHandleCount;
    int m_nEpollTimeOut;
};

#endif
