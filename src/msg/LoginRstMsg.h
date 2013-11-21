#ifndef _LOGINRSTMSG_H_
#define _LOGINRSTMSG_H_

#include "INetMsg.h"

enum class LoginRstType{ErrorID, ErrorPassword, Success};

class LoginRstMsg
    : public INetMsg
{
public:
    LoginRstMsg();
    virtual ~LoginRstMsg();
    LoginRstMsg(const LoginRstMsg &) = default;
    LoginRstMsg& operator=(const LoginRstMsg &) = default;
public:
    virtual std::string ClassName() const override;
    virtual IMsg* Clone() const override;
public:
    LoginRstType Type() const;
    void Type(LoginRstType type);
private:
    LoginRstType m_type;//登陆结果
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version);
};

template<typename Archive>
void LoginRstMsg::serialize(Archive &ar, const unsigned version)
{
    ar & boost::serialization::base_object<INetMsg>(*this);
    ar & m_type;
}

#endif
