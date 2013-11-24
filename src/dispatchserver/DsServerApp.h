#ifndef _BHSERVERAPP_H_
#define _BHSERVERAPP_H_

#include <string>
#include <boost/thread.hpp>
#include "../kernel/BhTcpServer.h"

class BhServerApp
{
private:
    BhServerApp();
public:
    ~BhServerApp();
    BhServerApp(const BhServerApp &) = delete;
    BhServerApp& operator=(const BhServerApp &) = delete;
public:
    static BhServerApp& Instance();
    bool Init(const std::string &strIP, int nPort, int nListenCount, int nEpollCount, int nEpollTimeOut, int nThreadCount, unsigned uBlockLen);
    void Clear();
    bool Start();
    void Stop();
    void Wait();
    bool HandleSockBuf(int nSock, BhMemeryPool &pool);
private:
    BhTcpServer m_tcpServer;
};

#endif
