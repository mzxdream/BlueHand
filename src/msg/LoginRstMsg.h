#ifndef _LOGINRSTMSG_H_
#define _LOGINRSTMSG_H_

#include "INetMsg.h"

class LoginRstMsg
    : public INetMsg
{
public:
    LoginRstMsg();
    virtual ~LoginRstMsg();
    LoginRstMsg(const LoginRstMsg &) = default;
    LoginRstMsg& operator=(const LoginRstMsg &) = default;
public:
    virtual std::string ClassName() const;
    virtual IMsg* Clone() const;
public:
    bool Success() const;
    void Success(bool);
private:
    bool m_bSuccess;
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version);
};

template<typename Archive>
void LoginRstMsg::serialize(Archive &ar, const unsigned version)
{
    ar & boost::serialization::base_object<INetMsg>(*this);
    ar & m_bSuccess;
}

#endif
