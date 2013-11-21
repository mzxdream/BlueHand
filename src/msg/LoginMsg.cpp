#include "LoginMsg.h"

BOOST_CLASS_EXPORT(LoginMsg)

LoginMsg::LoginMsg()
{
}
LoginMsg::LoginMsg(const std::string &strUserID, const std::string &strUserPwd)
    : m_strUserID(strUserID)
    , m_strUserPwd(strUserPwd)
{
}
LoginMsg::~LoginMsg()
{
}
std::string LoginMsg::ClassName() const
{
    return "LoginMsg";
}
IMsg* LoginMsg::Clone() const
{
    return new LoginMsg(*this);
}
const std::string& LoginMsg::UserID() const
{
    return m_strUserID;
}
void LoginMsg::UserID(const std::string &strUserID)
{
    m_strUserID = strUserID;
}
const std::string& LoginMsg::UserPwd() const
{
    return m_strUserPwd;
}
void LoginMsg::UserPwd(const std::string &strUserPwd)
{
    m_strUserPwd = strUserPwd;
}
