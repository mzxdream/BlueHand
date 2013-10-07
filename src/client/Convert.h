#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <string>

class Convert
{
public:
    Convert() = default;
    ~Convert() = default;
    Convert(const Convert &) = delete;
    Convert& operator=(const Convert &) = delete;
public:
    static std::string BoolToStr(bool);
    static std::string IntToStr(int);
    static std::string UIntToStr(unsigned);
    static std::string LongToStr(long);
    static std::string ULongToStr(unsigned long);
    static std::string LLToStr(long long);
    static std::string ULLToStr(unsigned long long);
    static std::string FloatToStr(float);
    static std::string DoubleToStr(double);
    static std::string LDoubleToStr(long double);
    
    static bool StrToBool(const std::string &, bool &);    
    static bool StrToInt(const std::string &, int &);
    static bool StrToUInt(const std::string &, unsigned &);
    static bool StrToLong(const std::string &, long &);
    static bool StrToULong(const std::string &, unsigned long &);
    static bool StrToLL(const std::string &, long long &);
    static bool StrToULL(const std::string &, unsigned long long &);
    static bool StrToFloat(const std::string &, float &);
    static bool StrToDouble(const std::string &, double &);
    static bool StrToLDouble(const std::string &, long double &);
};

#endif
