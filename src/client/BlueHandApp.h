#ifndef _BLUEHANDAPP_H_
#define _BLUEHANDAPP_H_

#include <gtkmm.h>
#include <iostream>
#include <string>
#include "LoginWnd.h"

class BlueHandApp
{
private:
    BlueHandApp() = default;
public:
    BlueHandApp(const BlueHandApp &) = delete;
    BlueHandApp& operator=(const BlueHandApp &) = delete;
    ~BlueHandApp() = default;
public:
    static BlueHandApp& Instance();
    bool Init();
    void Clear();
    bool Run();
    bool InitLoginWnd();
    bool InitConf();
    bool ReadLoginConf();
    bool WriteLoginConf();
public:
    void OnCancelLogin();
    void OnLogin(const std::string &, const std::string &, bool, bool);
private:
    //loginwnd
    std::string m_strID;
    std::string m_strPwd;
    bool m_bAutoLogin;
    bool m_bRmbPwd;
    LoginWnd *m_pLoginWnd;
};

#endif
