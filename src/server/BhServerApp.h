#ifndef _BHSERVERAPP_H_
#define _BHSERVERAPP_H_

#include <boost/ptr_container/ptr_unordered_map.hpp>
#include "../kernal/BhMemeryPool.h"
#include "../kernal/BhSockInfo.h"
#include <string>
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/bind.hpp>

class BhServerApp
{
public:
    typedef boost::shared_mutex RWMutex;
    typedef boost::shared_lock<RWMutex> ReadLock;
    typedef boost::unique_lock<RWMutex> WriteLock;
private:
    BhServerApp();
public:
    ~BhServerApp();
    BhServerApp(const BhServerApp &) = delete;
    BhServerApp& operator=(const BhServerApp &) = delete;
public:
    static BhServerApp& Instance();
    bool Init(const std::string &strIP, int nPort, int nListenCount, int nHandleCount, int nBlockLength, int nEpollTimeOut, int nMsgHeaderLen);
    void Clear();
    bool Start();
    void Stop();
    void Wait();
    void HandleMsg(int nEpoll, int nSock);
private:
    void Run();
private:
    boost::ptr_unordered_map<int, BhMemeryPool> m_sockBufPunmap;//sock读取的内容
    RWMutex m_sockBufMutex;
    boost::ptr_unordered_map<int, BhSockInfo> m_sockInfoPunmap;//sock信息
    RWMutex m_sockInfoMutex;
    std::string m_strIP;//绑定IP
    int m_nPort;//绑定端口
    int m_nListenCount;//同时最多监听sock数
    int m_nHandleCount;//同时最多处理sock数
    int m_nBlockLength;//sock读取的内容的分块长度
    bool m_bWantRun;//循环运行
    int m_nEpollTimeOut;//epoll wait超时
    int m_nMsgHeaderLen;//消息长度所占字节数
    boost::thread *m_pThread;
};

#endif
