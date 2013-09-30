#include "BlueHandApp.h"

BlueHandApp& BlueHandApp::Instance()
{
    static BlueHandApp app;
    return app;
}
bool BlueHandApp::Init()
{
    if (!InitLoginWnd())
    {
	return false;
    }
    return true;
}
bool BlueHandApp::InitLoginWnd()
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
