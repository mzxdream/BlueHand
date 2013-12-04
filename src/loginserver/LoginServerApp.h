#ifndef _LOGINSERVERAPP_H_
#define _LOGINSERVERAPP_H_

#include <string>
#include <boost/thread.hpp>
#include "../kernel/BhTcpServer.h"

class LoginServerApp
{
private:
    LoginServerApp();
public:
    ~LoginServerApp();
    LoginServerApp(const LoginServerApp &) = delete;
    LoginServerApp& operator=(const LoginServerApp &) = delete;
public:
    static LoginServerApp& Instance();
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
