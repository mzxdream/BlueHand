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
    void operator()();
private:
    std::string m_strLocalIP;//IP
    int m_nLocalPort;//端口
    int m_nListenMax;//同时监听最多sock
    int m_nHandleThreadMax;//同时处理最多sock
};

#endif
