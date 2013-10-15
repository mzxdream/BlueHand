#ifndef _BHSOCK_H_
#define _BHSOCK_H_

enum class EmSockType{Udp, Tcp};
class BhSock
{
public:
    BhSock();
    ~BhSock();
    BhSock(const BhSock &) = delete;
    BhSock& operator=(const BhSock &) = delete;
public:
    bool Init();
    void Clear();
    bool Create(EmSockType);
    void Destroy();
    int Detach();
    bool Attach();
    bool Bind(int);
    bool Bind(const std::string &, int);
    bool Listen(int);
    bool Accept(BhSock &);
private:
    int m_sock;
};

#endif
