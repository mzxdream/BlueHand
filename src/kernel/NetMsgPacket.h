#ifndef _NETMSGPACKET_H_
#define _NETMSGPACKET_H_

#include "../msg/INetMsg.h"

class NetMsgPacket
{
public:
    static INetMsg* UnPack(const char *pBuf, int nLen);
    static char* Packet(int &nLen, const INetMsg *pMsg);
};


#endif
