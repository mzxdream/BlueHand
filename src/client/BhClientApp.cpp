#include "BhClientApp.h"
#include "Conf.h"

BhClientApp& BhClientApp::Instance()
{
    static BhClientApp app;
    return app;
}

bool BhClientApp::Init()
{
    if (!InitLoginWnd())
    {
	return false;
    }
    return true;
}
bool BhClientApp::InitLoginWnd()
{
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try
    {
	refBuilder->add_from_file(UI_LOGINWND);
    }
    catch (const Glib::FileError &ex)
    {
	return false;
    }
    catch (const Glib::MarkupError &ex)
    {
	return false;
    }
    catch (const Gtk::BuilderError &ex)
    {
	return false;
    }
    refBuilder->get_widget_derived("LoginWnd", m_pLoginWnd);
    if (!m_pLoginWnd || !m_pLoginWnd->Init())
    {
	return false;
    }
    m_strServerAddr = Conf::GetString("./conf/login.ini", "userset", "serveraddr", "");
    m_strUserID = Conf::GetString("./conf/login.ini", "userinfo", "userid", "");
    m_strUserPwd = Conf::GetString("./conf/login.ini", "userinfo", "userpassword", "");
    m_bAutoLogin = Conf::GetBool("./conf/login.ini", "userset", "autologin", false);
    m_bRmbPwd = Conf::GetBool("./conf/login.ini", "userset", "rememberpwd", false);
    m_pLoginWnd->ServerAddr(m_strServerAddr);
    m_pLoginWnd->UserID(m_strUserID);
    m_pLoginWnd->UserPwd(m_strUserPwd);
    m_pLoginWnd->AutoLogin(m_bAutoLogin);
    m_pLoginWnd->RememberPwd(m_bRmbPwd);
    m_pLoginWnd->CancelLoginSignal().connect(
	boost::bind(&BlueHandApp::OnCancelLogin, this));
    m_pLoginWnd->LoginSignal().connect(
	boost::bind(&BlueHandApp::OnLogin, this));
    return true;
}
void BhClientApp::Clear()
{
    if (m_pLoginWnd)
    {
	delete m_pLoginWnd;
	m_pLoginWnd = nullptr;
    }
}
bool BhClientApp::Run()
{
    if (!m_pLoginWnd)
    {
	return false;
    }
    Gtk::Main::run(*m_pLoginWnd);
    return true;
}
void BhClientApp::OnCancelLogin()
{
    std::cout << "cancel login" << std::endl;
}
void BhClientApp::OnLogin()
{
    m_strServerAddr = m_pLoginWnd->ServerAddr();
    m_strUserID = m_pLoginWnd->UserID();
    m_strUserPwd = m_pLoginWnd->UserPwd();
    m_bAutoLogin = m_pLoginWnd->AutoLogin();
    m_bRmbPwd = m_pLoginWnd->RememberPwd();

    Conf::WriteString("./conf/login.ini", "userset", "serveraddr", m_strServerAddr);
    Conf::WriteString("./conf/login.ini", "userinfo", "userid", m_strUserID);
    Conf::WriteString("./conf/login.ini", "userinfo", "userpassword", m_strUserPwd);
    Conf::WriteBool("./conf/login.ini", "userset", "autologin", m_bAutoLogin);
    Conf::WriteBool("./conf/login.ini", "userset", "rememberpwd", m_bRmbPwd);
}
