#include "Conf.h"

bool Conf::GetConfStr(const std::string &strFile, const std::string &strSection, const std::string &strKey, std::string &strConf)
{
    if (strFile.empty()
	|| strSection.empty()
	|| strKey.empty())
    {
	return false;
    }
    std::ifstream ifs;
    ifs.open(strFilePath);
    if (!ifs.is_open())
    {
	return false;
    }
    char cBuf[CONF_MAX_LINE + 1] = {0};
    std::string strBuf;
    while (ifs.getline(cBuf, CONF_MAX_LINE))
    {
	strBuf = cBuf;
	if (strBuf.length() == (strSection.length() + 2)
	    && '[' == strBuf.at(0)
	    && ']' == strBuf.at(strSection.length() + 1)
	    && !strBuf.compare(1, strSection.length(), strSection))
	{
	    while (ifs.getline(cBuf, CONF_MAX_LINE))
	    {
		strBuf = cBuf;
		if (0 == strBuf.length()
		    || '[' == strBuf.at(0))
		{
		    break;
		}
		if (strBuf.length() > strKey.length()
		    && '=' == strBuf.at(strkey.length())
		    && !strBuf.compare(0, strKey.length(), strKey))
		{
		    if (strBuf.length() > strKey.length() + 1)
		    {
			strConf = strBuf.substr(strKey.length() + 1);
			return true;
		    }
		    break;
		}
	    }
	    break;
	}
    }
    return false;
}

bool Conf::GetBool(const std::string &strFile, const std::string &strSection, const std::string &strKey, bool bDefault)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	
    }
    return bDefault;
}
bool Conf::WriteBool(const std::string &strFile, const std::string &strSection, const std::string &strKey, bool bResult)
{
    return false;
}
int Conf::GetInt(const std::string &strFile, const std::string &strSection, const std::string &strKey, int nDefault)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
    }
    return nDefault;
}
bool Conf::WriteInt(const std::string &strFile, const std::string &strSection, const std::string &strKey, bool bResult)
{
    return false;
}
std::string Conf::GetString(const std::string &strFile, const std::string &strSection, const std::string &strKey, const std::string &strDefault)
{
    std::string strConf(strDefault);
    Conf::GetConfStr(strFile, strSection, strKey, strConf);
    return strConf;
}
bool Conf::WriteString(const std::string &strFile, const std::string &strSection, const std::string &strKey, const std::string &strResult)
{
    return false;
}
