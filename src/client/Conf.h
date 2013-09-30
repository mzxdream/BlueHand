#ifndef _CONF_H_
#define _CONF_H_

class Conf
{
private:
    Conf() = default;
public:
    ~Conf() = default;
    Conf(const Conf &) = delete;
    Conf& operator=(const Conf &) = delete;
public:
    static Conf& Instance();
    bool GetBool(const std::string &, const std::string &, const std::string &, bool);
    int GetInt(const std::string &, const std::string &, const std::string &, int);
    const std::string& GetString(const std::string &, const std::string &, const std::string &, const std::string &);
};

#endif
