#include "BhSocket.h"

bool BhSocket::Bind(int nSock, int nPort)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(nPort);
    addr.sin_addr.s_addr = INADDR_ANY;
    return bind(nSock, (struct sockaddr *)&addr, sizeof(addr)) >= 0; 
}
bool BhScoket::SetNonBlock(int nSock)
{
    int nFlags = 0;
    if ((nFlags = fcntl(nSock, F_GETFL, 0)) != -1)
    {
	if (fcntl(nSock, F_SETFL, nFlags | O_NONBLOCK) != -1)
	{
	    return true;
	}
    }
    return false;
}
int BhSocket::RecvAllLen(int nSock, void *pBuf, int nLen)
{
    int nRecvLen = 0;
    ssize_t sockRet = 0;
    while (true)
    {
	sockRet = recv(nSock, pBuf + nRecvLen, nLen - nRecvLen);
	if (sockRet < 0)
	{
	   if (errno == EAGAIN
		|| errno == EWOULDBLOCK
		|| errno == EINTR)
	    {
		if (0 == nRecvLen)
		{
		    return 0;
		}
		else
		{
		    continue;
		}
	    }
	    else
	    {
		return -1;
	    }
	}
	else if (0 == sockRet)
	{
	    return -1;
	}
	else
	{
	    nRecvlen += sockRet;
	    if (nRecvLen == nLen)
	    {
		return 1;
	    }
	}
    }
    return -1;
}
