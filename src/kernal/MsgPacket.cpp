#include "MsgPacket.h"
#include <sstream>

IMsg* MsgPacket::UnPack(const char *pBuf, int nLen)
{
    if (pBuf == nullptr || nLen <= 0)
    {
	return nullptr;
    }
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    ss.write(pBuf, nLen);
    boost::archive::binary_iarchive ia(ss);
    IMsg *pMsg = nullptr;
    try
    {
	ia >> pMsg;
    }
    catch (...)
    {
    }
    return pMsg;
}
char* MsgPacket::Packet(int &nLen, const IMsg* pMsg)
{
    if (pMsg == nullptr)
    {
	return nullptr;
    }
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    boost::archive::binary_oarchive oa(ss);
    oa << pMsg;
    nLen = ss.str().size();
    char *pBuf = new char[nLen];
    ss.seekp(0, std::ios::beg);
    ss.write(pBuf, nLen);
    return pBuf;
}
