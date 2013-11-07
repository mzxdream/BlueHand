#ifndef _BHSOCKET_H_
#define _BHSOCKET_H_

class BhSocket
{
public:
    BhSocket() = default;
    ~BhSocket() = default;
    BhSocket(const BhSocket &) = delete;
    BhSocket& operator=(const BhSocket &) = delete;
public:
    static bool Bind(int nSock, int nPort);
    static bool SetNonBlock(int nSock);
    static int RecvAllLen(int nSock, void *pBuf, int nLen);
};

#endif
