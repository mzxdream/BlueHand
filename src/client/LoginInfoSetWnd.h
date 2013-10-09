#ifndef _LOGININFOSETWND_H_
#define _LOGININFOSETWND_H_

class LoginInfoSetWnd
    : public Gtk::Window
{
public:
    LoginInfoSetWnd(BaseObjectType *, const Glib::RefPtr<Gtk::Builder> &);
    ~LoginInfoSetWnd();
    LoginInfoSetWnd(const LoginInfoSetWnd &) = delete;
    LoginInfoSetWnd& operator=(const LoginInfoSetWnd &) = delete;
public:
    bool Init();
    void Clear();
    std::string& ServerIP() const;
    void ServerIP(const std::string &);
    unsigned short ServerPort() const;
    void SetServerPort(unsigned short);
};

#endif
