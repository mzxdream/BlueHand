#ifndef _LOGINMSG_H_
#define _LOGINMSG_H_

#include "INetMsg.h"

class LoginMsg
    :public INetMsg
{
public:
    LoginMsg();
    virtual ~LoginMsg();
    LoginMsg(const LoginMsg &) = default;
    LoginMsg& operator=(const LoginMsg &) = default;
public:
    virtual std::string ClassName() const;
    virtual IMsg* Clone() const;
public:
private:
private:
    
};

#endif
