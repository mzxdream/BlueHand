#ifndef _MSGPACKET_H_
#define _MSGPACKET_H_

#include "IMsg.h"

class MsgPacket
{
public:
    static IMsg* UnPack(const void *pBuf, int nLen);
    static void* Packet(int nLen, const IMsg *pMsg);
};


#endif
