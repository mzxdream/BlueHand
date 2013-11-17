#ifndef _BHCLIENTAPP_H_
#define _BHCLIENTAPP_H_

#include <gtkmm.h>
#include <iostream>
#include <string>
#include "LoginWnd.h"

class BhClientApp
{
private:
    BhClientApp() = default;
public:
    BhClientApp(const BhClientApp &) = delete;
    BhClientApp& operator=(const BhClientApp &) = delete;
    ~BhClientApp() = default;
public:
    static BhClientApp& Instance();
    bool Init();
    void Clear();
    bool Run();
    bool InitLoginWnd();
public:
    void OnCancelLogin();
    void OnLogin();
private:
    //loginwnd
    std::string m_strServerAddr;
    std::string m_strUserID;
    std::string m_strUserPwd;
    bool m_bAutoLogin;
    bool m_bRmbPwd;
    LoginWnd *m_pLoginWnd;
};

#endif
