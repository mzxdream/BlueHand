#ifndef _LOGINWND_H_
#define _LOGINWND_H_

#include <gtkmm.h>
#include <boost/signals2.hpp>
#include <string>
#include <iostream>
#include "UIDefine.h"

class LoginWnd
    : public Gtk::Window
{
public:
    LoginWnd(BaseObjectType *, const Glib::RefPtr<Gtk::Builder> &);
    ~LoginWnd();
    LoginWnd(const LoginWnd &) = delete;
    LoginWnd& operator=(const LoginWnd &) = delete;
public:
    bool Init();
    void Clear();
    const std::string& UserID() const;
    void UserID(const std::string &);
    const std::string& UserPwd() const;
    void UserPwd(const std::string &);
    bool RememberPwd() const;
    void RememberPwd(bool);
    bool AutoLogin() const;
    void AutoLogin(bool);
    boost::signals2::signal<void ()>& CloseSignal();
    boost::signals2::signal<void ()>& CancelLoginSignal();
    boost::signals2::signal<void ()>& LoginSignal();
    boost::signals2::signal<void ()>& SetSignal();
protected:
    void OnSetBtnClicked();
    void OnLoginBtnClicked();
protected:
    boost::signals2::signal<void ()> m_closeSig;
    boost::signals2::signal<void ()> m_cancelLoginSig;
    boost::signals2::signal<void ()> m_loginSig;
    boost::signals2::signal<void ()> m_setSig;
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Gtk::Button *m_pLoginBtn;
    Gtk::Button *m_pSetBtn;
    Gtk::Label *m_pIDLabel;
    Gtk::Label *m_pPwdLabel;
    Gtk::Entry *m_pIDEntry;
    Gtk::Entry *m_pPwdEntry;
    Gtk::CheckButton *m_pRmbPwdChkBtn;
    Gtk::CheckButton *m_pAutoLoginChkBtn;
    bool m_bLogin;
};

#endif
