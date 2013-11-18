#ifndef _BHTCPSERVER_H_
#define _BHTCPSERVER_H_

#include "BhSocket.h"
#include <string>
#include <boost/ptr_container/ptr_list.hpp>
#include "BhSockInfo.h"
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/bind.hpp>

class BhTcpServer
{
public:
    typedef boost::shared_mutex RWMutex;
    typedef boost::shared_lock<RWMutex> ReadLock;
    typedef boost::unique_lock<RWMutex> WriteLock;
public:
    BhTcpServer();
    ~BhTcpServer();
    BhTcpServer& operator=(const BhTcpServer &) = delete;
    BhTcpServer(const BhTcpServer &) = delete;
public:
    bool Init(const std::string &strIP, int nPort, int nListenCount, int nEpollCount, int nEpollTimeOut, int nHandleThreadCount);
    void Clear();
    bool Start();
    void Stop();
    void Wait();
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
    boost::ptr_list<BhSockInfo> m_sockInfoList;//
    bool m_bWantRun;
};

#endif
