#ifndef _CONF_H_
#define _CONF_H_

#include <string>

#define CONF_MAX_LINE 1024//配置文件一行最多1024个

class Conf
{
public:
    Conf() = default;
    ~Conf() = default;
    Conf(const Conf &) = delete;
    Conf& operator=(const Conf &) = delete;
public:
    static bool GetBool(const std::string &, const std::string &, const std::string &, bool);
    static int GetInt(const std::string &, const std::string &, const std::string &, int);
    static unsigned GetUInt(const std::string &, const std::string &, const std::string &, unsigned);
    static long GetLong(const std::string &, const std::string &, const std::string &, long);
    static unsigned long GetULong(const std::string &, const std::string &, const std::string &, unsigned long);
    static long long GetLL(const std::string &, const std::string &, const std::string &, long long);
    static unsigned long long GetULL(const std::string &, const std::string &, const std::string &, unsigned long long);
    static float GetFloat(const std::string &, const std::string &, const std::string &, float);
    static double GetDouble(const std::string &, const std::string &, const std::string &, double);
    static long double GetLDouble(const std::string &, const std::string &, const std::string &, long double);
    static std::string GetString(const std::string &, const std::string &, const std::string &, const std::string &);
    
    static bool WriteBool(const std::string &, const std::string &, const std::string &, bool);
    static bool WriteInt(const std::string &, const std::string &, const std::string &, int);
    static bool WriteUInt(const std::string &, const std::string &, const std::string &, unsigned);
    static bool WriteLong(const std::string &, const std::string &, const std::string &, long);
    static bool WriteULong(const std::string &, const std::string &, const std::string &, unsigned long);
    static bool WriteLL(const std::string &, const std::string &, const std::string &, long long);
    static bool WriteULL(const std::string &, const std::string &, const std::string &, unsigned long long);
    static bool WriteFloat(const std::string &, const std::string &, const std::string &, float);
    static bool WriteDouble(const std::string &, const std::string &, const std::string &, double);
    static bool WriteLDouble(const std::string &, const std::string &, const std::string &, long double);
    static bool WriteString(const std::string &, const std::string &, const std::string &, const std::string &);
private:
    static bool GetConfStr(const std::string &, const std::string &, const std::string &, std::string &);
};

#endif
