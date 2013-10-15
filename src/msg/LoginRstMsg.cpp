#include "LoginRstMsg.h"

BOOST_CLASS_EXPORT(LoginRstMsg)

LoginRstMsg::LoginRstMsg()
{
}
LoginRstMsg::~LoginRstMsg()
{
}
std::string LoginRstMsg::ClassName() const
{
    return "LoginRstMsg";
}
IMsg* LoginRstMsg::Clone() const
{
    return new LoginMsg(*this);
}
bool LoginRstMsg::Success() const
{
    return m_bSuccess;
}
void LoginRstMsg::Success(bool bSuccess)
{
    m_bSuccess = bSuccess;
}
