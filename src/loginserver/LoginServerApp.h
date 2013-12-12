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
    bool HandleLoginSockBuf(int nSock, BhMemeryPool &pool);
    bool HandleCenterSockBuf(int nSock, BhMemeryPool &pool);
private:
    BhTcpServer m_loginServer;//对用户的登录进行处理
    BhTcpServer m_centerServer;//对中心服务器的处理
};

#endif
