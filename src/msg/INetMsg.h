#ifndef _INETMSG_H_
#define _INETMSG_H_

#include "IMsg.h"
#include <string>

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
    const std::string& NetID() const;
    void NetID(const std::string &strNetID);
private:
    std::string m_strNetID;//登陆成功，由服务器发过来的唯一标识
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version);
};

template<typename Archive>
void INetMsg::serialize(Archive &ar, const unsigned version)
{
    ar & boost::serialization::base_object<IMsg>(*this);
    ar & m_strNetID;
}
#endif
