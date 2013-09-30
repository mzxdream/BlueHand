#include "LoginWnd.h"

LoginWnd::LoginWnd(BaseObjectType *pObj, const Glib::RefPtr<Gtk::Builder> &refGlade)
    : Gtk::Window(pObj)
    , m_refBuilder(refGlade)
    , m_pLoginBtn(nullptr)
    , m_pSetBtn(nullptr)
    , m_pIDLabel(nullptr)
    , m_pPwdLabel(nullptr)
    , m_pIDEntry(nullptr)
    , m_pPwdEntry(nullptr)
    , m_pRmbPwdChkBtn(nullptr)
    , m_pAutoLoginChkBtn(nullptr)
    , m_bLogin(true)
{
}
LoginWnd::~LoginWnd()
{
}
bool LoginWnd::Init()
{
    m_refBuilder->get_widget("LoginBtn", m_pLoginBtn);
    m_refBuilder->get_widget("SetBtn", m_pSetBtn);
    m_refBuilder->get_widget("IDLabel", m_pIDLabel);
    m_refBuilder->get_widget("PwdLabel", m_pPwdLabel);
    m_refBuilder->get_widget("IDEntry", m_pIDEntry);
    m_refBuilder->get_widget("PwdEntry", m_pPwdEntry);
    m_refBuilder->get_widget("RmbPwdChkBtn", m_pRmbPwdChkBtn);
    m_refBuilder->get_widget("AutoLoginChkBtn", m_pAutoLoginChkBtn);
   
    if (!m_pLoginBtn
	|| !m_pSetBtn
	|| !m_pIDLabel
	|| !m_pPwdLabel
	|| !m_pIDEntry
	|| !m_pPwdEntry
	|| !m_pRmbPwdChkBtn
	|| !m_pAutoLoginChkBtn
	)
    {
	return false;
    }
    m_pLoginBtn->signal_clicked().connect(
	sigc::mem_fun(*this, &LoginWnd::OnLoginBtnClicked));
    m_pSetBtn->signal_clicked().connect(
	sigc::mem_fun(*this, &LoginWnd::OnSetBtnClicked));
    
    return true;
}
void LoginWnd::Clear()
{
}

void LoginWnd::OnSetBtnClicked()
{
}
void LoginWnd::OnLoginBtnClicked()
{
    if (m_bLogin)
    {
	m_pLoginBtn->set_label("Logining");
	m_bLogin = false;
	m_loginSig(m_pIDEntry->get_text().raw(), m_pPwdEntry->get_text().raw()
		   , m_pRmbPwdChkBtn->get_active(), m_pAutoLoginChkBtn->get_active());
    }
    else
    {
	m_pLoginBtn->set_label("Login");
	m_bLogin = true;
	m_cancelLoginSig();
    }
}

