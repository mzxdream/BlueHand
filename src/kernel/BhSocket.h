#ifndef _BHSOCKET_H_
#define _BHSOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

enum SockType{TCP, UDP};

class BhSocket
{
public:
    explicit BhSocket(int nSock = -1);
    ~BhSocket();
    BhSocket(const BhSocket &) = delete;
    BhSocket& operator=(const BhSocket &) = delete;
public:
    void Attach(int nSock);
    int Detach();
    bool Create(SockType type);
    bool Bind(int nPort, const std::string &strIP = "");
    bool Listen(int nCount);
    bool SetNonBlock();
    void Destory();
public:
    static bool SetNonBlock(int nSock);
private:
    int m_nSock;
};

#endif
