#include "INetMsg.h"

BOOST_SERIALIZATION_ASSUME_ABSTRACT(INetMsg)

INetMsg::INetMsg()
    : m_uNetID(0)
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
