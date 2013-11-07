#ifndef _BHSERVERAPP_H_
#define _BHSERVERAPP_H_

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
    static void HandleMsg(int nSock, int nEpoll);
private:
    std::list<int> m_sockList;//sock
    int m_nPort;//绑定端口
    int m_nListenCount;//同时最多监听sock数
    int m_nHandleCount;//同时最多处理sock数
};

#endif
