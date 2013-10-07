#include "BlueHandApp.h"
#include "Conf.h"

BlueHandApp& BlueHandApp::Instance()
{
    static BlueHandApp app;
    return app;
}

bool BlueHandApp::Init()
{
    if (!InitConf())
    {
	return false;
    }
    if (!InitLoginWnd())
    {
	return false;
    }
    return true;
}
bool BlueHandApp::InitConf()
{
    return Conf::Init();
}
bool BlueHandApp::ReadLoginConf()
{
    m_strID = Conf::GetString(CONF_LOGIN, "UserInfo", "ID", "");
    m_strPwd = Conf::GetString(CONF_LOGIN, "UserInfo", "PWD", "");
    m_bAutoLogin = Conf::GetBool(CONF_LOGIN, "UserSet", "AutoLogin", false);
    m_bRmbPwd = Conf::GetBool(CONF_LOGIN, "UserSet", "RmbPwd", false);
    return true;
}
bool BlueHandApp::WriteLoginConf()
{
    return true;
}
bool BlueHandApp::InitLoginWnd()
{
    if (!ReadLoginConf())
    {
	return false;
    }
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
    m_pLoginWnd->SetUserID(m_strID);
    m_pLoginWnd->SetPwd(m_strPwd);
    m_pLoginWnd->SetAutoLogin(m_bAutoLogin);
    m_pLoginWnd->SetRmbPwdChk(m_bRmbPwd);
    m_pLoginWnd->m_cancelLoginSig.connect(
	boost::bind(&BlueHandApp::OnCancelLogin, this));
    m_pLoginWnd->m_loginSig.connect(
	boost::bind(&BlueHandApp::OnLogin, this, _1, _2, _3, _4));
    return true;
}
void BlueHandApp::Clear()
{
}
bool BlueHandApp::Run()
{
    if (!m_pLoginWnd)
    {
	return false;
    }
    Gtk::Main::run(*m_pLoginWnd);
    return true;
}
void BlueHandApp::OnCancelLogin()
{
    std::cout << "cancel login" << std::endl;
}
void BlueHandApp::OnLogin(const std::string &strID, const std::string &strPwd, bool bRmbPwd, bool bAutoLogin)
{
    std::cout << strID << strPwd << (bRmbPwd ? "TRUE" : "FALSE") << (bAutoLogin ? "TRUE" : "FALSE") << std::endl;
}
