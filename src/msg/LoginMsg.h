#ifndef _LOGINMSG_H_
#define _LOGINMSG_H_

#include "INetMsg.h"

class LoginMsg
    :public INetMsg
{
public:
    LoginMsg();
    LoginMsg(const std::string &, const std::string &);
    virtual ~LoginMsg();
    LoginMsg(const LoginMsg &) = default;
    LoginMsg& operator=(const LoginMsg &) = default;
public:
    virtual std::string ClassName() const;
    virtual IMsg* Clone() const;
public:
    const std::string& UserID() const;
    void UserID(const std::string &);
    const std::string& UserPwd() const;
    void UserPwd(const std::string &);
private:
    std::string m_strUserID;
    std::string m_strUserPwd;
private:
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive &ar, const unsigned version);
};

template<typename Archive>
void LoginMsg::serialize(Archive &ar, const unsigned version)
{
    ar & boost::serialization::base_object<INetMsg>(*this);
    ar & m_strUserID;
    ar & m_strUserPwd;
}

#endif
