#ifndef _BHTCPSERVER_H_
#define _BHTCPSERVER_H_

#include "BhSocket.h"
#include <string>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include "BhMemeryPool.h"
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/bind.hpp>

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
    bool Start();
    void Stop();
    void Wait();
private:
    int m_nListenSock;
    int m_nEpoll;
    int m_nListenCount;
    int m_nHandleCount;
    int m_nEpollTimeOut;
};

#endif
