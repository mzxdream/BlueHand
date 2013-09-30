#include "Conf.h"

Conf& Conf::Instance()
{
    static Conf conf;
    return conf;
}

bool Conf::GetBool(const std::string &strFilePath, const std::string &strAppName, const std::string &strKeyName, bool bDefault)
{
    return bDefault;
}
int Conf::GetInt(const std::string &strFilePath, const std::string &strAppName, const std::string &strKeyName, int nDefault)
{
    return nDefault;
}
const std::string& Conf::GetString(const std::string &strFilePath, const std::string &strAppName, const std::string &strKeyName, const std::string & strDefault)
{
    return strDefault;
}
