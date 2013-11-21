#include "INetMsg.h"

BOOST_SERIALIZATION_ASSUME_ABSTRACT(INetMsg)

INetMsg::INetMsg()
{
}
INetMsg::~INetMsg()
{
}
std::string INetMsg::ClassName() const
{
    return "INetMsg";
}
const std::string& INetMsg::NetID() const
{
    return m_strNetID;
}
void INetMsg::NetID(const std::string &strNetID)
{
    m_strNetID = strNetID;
}
