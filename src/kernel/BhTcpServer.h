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
    typedef boost::function<bool (int ,BhMemeryPool &)> InvokeFunc;
public:
    explicit BhTcpServer(const InvokeFunc &func);
    BhTcpServer();
    ~BhTcpServer();
    BhTcpServer& operator=(const BhTcpServer &) = delete;
    BhTcpServer(const BhTcpServer &) = delete;
public:
    bool Init(const std::string &strIP, int nPort, int nListenCount, int nEpollCount, int nEpollTimeOut, int nThreadCount, unsigned uBlockLen);
    void Clear();
    bool Start();
    void Stop();
    void Wait();
    void SetFunc(InvokeFunc func);
    void HandleMsg(int nEpoll, int nSock);
private:
    void Run();
    void CloseSock(int nSock);
private:
    std::string m_strIP;//绑定IP
    int m_nPort;//绑定port
    int m_nListenCount;//监听数量
    int m_nEpollCount;//epoll wait count
    int m_nEpollTimeOut;//epoll 超时
    int m_nThreadCount;//处理线程数量
    unsigned m_uBlockLen;//sock读取的buf大小
    boost::thread *m_pThread;
    InvokeFunc m_invokeFunc;
    bool m_bWantRun;
    RWMutex m_sockInfoMutex;//sock锁
    boost::ptr_list<BhSockInfo> m_sockInfoPlist;//
};

#endif
