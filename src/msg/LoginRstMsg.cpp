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
    return new LoginRstMsg(*this);
}
LoginRstType LoginRstMsg::Type() const
{
    return m_type;
}
void LoginRstMsg::Type(LoginRstType type)
{
    m_type = type;
}
