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
    //loginSet
    
};

#endif
