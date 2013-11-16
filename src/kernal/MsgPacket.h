#ifndef _MSGPACKET_H_
#define _MSGPACKET_H_

#include "../msg/IMsg.h"

class MsgPacket
{
public:
    static IMsg* UnPack(const char *pBuf, int nLen);
    static char* Packet(int &nLen, const IMsg *pMsg);
};


#endif
