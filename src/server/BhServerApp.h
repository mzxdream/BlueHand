#ifndef _BHSERVERAPP_H_
#define _BHSERVERAPP_H_

#include <boost/ptr_container/ptr_unordered_map.hpp>
#include "BhMemeryPool.h"

class BhServerApp
{
private:
    BhServerApp() = default;
public:
    ~BhServerApp() = default;
    BhServerApp(const BhServerApp &) = delete;
    BhServerApp& operator=(const BhServerApp &) = delete;
public:
    static BhServerApp& Instance();
    bool Init();
    void Clear();
    void Run();
    static int CreateSock();
    static int BindSock();
    static bool SetSockNonBlock(int nSock);
    static int RecvAll(int nSock, void *pBuf, int nLen);
    void HandleMsg(int nSock, int nEpoll);
private:
    boost::ptr_unordered_map<int, BhMemeryPool> m_sockBufUnmap;//sock读取的内容
    std::string m_strIP;//绑定IP
    int m_nPort;//绑定端口
    int m_nListenCount;//同时最多监听sock数
    int m_nHandleCount;//同时最多处理sock数
    int m_nBlockLength;//sock读取的内容的分块长度
    bool m_bWantRun;//socket运行
};

#endif
