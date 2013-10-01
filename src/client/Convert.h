#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <sstream>
#include <string>

class Convert
{
public:
    Convert() = default;
    ~Convert() = default;
    Convert(const Convert &) = delete;
    Convert& operator=(const Convert &) = delete;
public:
    static bool IntToStr(int, std::string &);
    static bool StrToInt(const std::string &, int &);
    static bool BoolToStr(bool, std::string &);
    static bool StrToBool(const std::string &, bool &);
};

#endif
