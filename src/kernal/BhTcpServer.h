#ifndef _BHTCPSERVER_H_
#define _BHTCPSERVER_H_

#include "BhSocket.h"
#include <string>
#include <boost/ptr_container/ptr_list.hpp>
#include "BhSockInfo.h"
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class BhTcpServer
{
public:
    typedef boost::shared_mutex RWMutex;
    typedef boost::shared_lock<RWMutex> ReadLock;
    typedef boost::unique_lock<RWMutex> WriteLock;
    typedef boost::function<bool (BhMemeryPool &)> InvokeFunc;
public:
    explicit BhTcpServer(const InvokeFunc &func);
    ~BhTcpServer();
    BhTcpServer& operator=(const BhTcpServer &) = delete;
    BhTcpServer(const BhTcpServer &) = delete;
public:
    bool Init(const std::string &strIP, int nPort, int nListenCount, int nEpollCount, int nEpollTimeOut, int nHandleThreadCount);
    void Clear();
    bool Start();
    void Stop();
    void Wait();
    void SetFunc(InvokeFunc func);
    void HandleMsg(int nSock);
private:
    void Run();
private:
    std::string m_strIP;//绑定IP
    int m_nPort;//绑定port
    int m_nListenSock;//监听sock
    int m_nListenCount;//监听数量
    int m_nEpoll;//epoll id
    int m_nEpollCount;//epoll wait count
    int m_nEpollTimeOut;//epoll 超时
    int m_nThreadCount;//处理线程数量
    RWMutex m_sockInfoMutex;//sock锁
    boost::ptr_list<BhSockInfo> m_sockInfoPlist;//
    bool m_bWantRun;
    InvokeFunc m_funcInvoke;
};

#endif
