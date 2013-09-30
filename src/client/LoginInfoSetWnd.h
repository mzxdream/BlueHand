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
    const std::string& GetLocalIP() const;
    void SetLocalIP(const std::string &);
    unsigned GetLocalPort() const;
    void SetLocalPort(unsigned);
    const std::string& GetServerIP() const;
    void SetServerIP(const std::string &);
    unsigned GetServerPort() const;
    void SetServerPort(unsigned);
};

#endif
