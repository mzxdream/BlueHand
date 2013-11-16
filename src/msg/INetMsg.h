#ifndef _INETMSG_H_
#define _INETMSG_H_

#include "IMsg.h"

class INetMsg
    :public IMsg
{
public:
    INetMsg();
    virtual ~INetMsg();
    INetMsg(const INetMsg &) = default;
    INetMsg& operator=(const INetMsg &) = default;
public:
    virtual std::string ClassName() const;
    virtual IMsg* Clone() const = 0;
public:
    unsigned NetID() const;
    void NetID(unsigned);
private:
    unsigned m_uNetID;
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version);
};

template<typename Archive>
void INetMsg::serialize(Archive &ar, const unsigned version)
{
    ar & boost::serialization::base_object<IMsg>(*this);
    ar & m_uNetID;
}
#endif
