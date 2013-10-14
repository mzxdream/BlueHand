#include "INetMsg.h"

BOOST_SERIALIZATION_ASSUME_ABSTRACT(INetMsg)

INetMsg::INetMsg()
    : m_uNetID(0)
    , m_uPacketNum(0)
{
}
INetMsg::~INetMsg()
{
}
std::string INetMsg::ClassName() const
{
    return "INetMsg";
}
unsigned INetMsg::NetID() const
{
    return m_uNetID;
}
void INetMsg::NetID(unsigned uNetID)
{
    m_uNetID = uNetID;
}
unsigned INetMsg::PacketNum() const
{
    return m_uPacketNum;
}
void INetMsg::PacketNum(unsigned uPacketNum)
{
    m_uPacketNum = uPacketNum;
}
