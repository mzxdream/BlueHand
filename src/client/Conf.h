#ifndef _CONF_H_
#define _CONF_H_

#include <fstream>

#define CONF_MAX_LINE 1024

class Conf
{
public:
    Conf() = default;
    ~Conf() = default;
    Conf(const Conf &) = delete;
    Conf& operator=(const Conf &) = delete;
public:
    static bool GetBool(const std::string &, const std::string &, const std::string &, bool);
    static bool WriteBool(const std::string &, const std::string &, const std::string &, bool);
    static int GetInt(const std::string &, const std::string &, const std::string &, int);
    static bool WriteInt(const std::string &, const std::string &, const std::string &, int);
    static std::string GetString(const std::string &, const std::string &, const std::string &, const std::string &);
    static bool WriteString(const std::string &, const std::string &, const std::string &, const std::string &);
protected:
    
};

#endif
