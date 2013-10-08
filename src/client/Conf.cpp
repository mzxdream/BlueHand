#include "Conf.h"
#include <fstream>
#include <streambuf>
#include "Convert.h"
#include <boost/filesystem.hpp>

bool Conf::GetConfStr(const std::string &strFile, const std::string &strSection, const std::string &strKey, std::string &strConf)
{
    if (strFile.empty()
	|| strSection.empty()
	|| strKey.empty())
    {
	return false;
    }
    std::ifstream ifs;
    ifs.open(strFile);
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
		    && '=' == strBuf.at(strKey.length())
		    && !strBuf.compare(0, strKey.length(), strKey))
		{
		    if (strBuf.length() > strKey.length() + 1)
		    {
			strConf = strBuf.substr(strKey.length() + 1);
			ifs.close();
			return true;
		    }
		    break;
		}
	    }
	    break;
	}
    }
    ifs.close();
    return false;
}

bool Conf::GetBool(const std::string &strFile, const std::string &strSection, const std::string &strKey, bool bDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToBool(strConf, bDef);
    }
    return bDef;
}
int Conf::GetInt(const std::string &strFile, const std::string &strSection, const std::string &strKey, int nDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToInt(strConf, nDef);
    }
    return nDef;
}
unsigned Conf::GetUInt(const std::string &strFile, const std::string &strSection, const std::string &strKey, unsigned uDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToUInt(strConf, uDef);
    }
    return uDef;
}
long Conf::GetLong(const std::string &strFile, const std::string &strSection, const std::string &strKey, long lDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToLong(strConf, lDef);
    }
    return lDef;
}
unsigned long Conf::GetULong(const std::string &strFile, const std::string &strSection, const std::string &strKey, unsigned long ulDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToULong(strConf, ulDef);
    }
    return ulDef;
}
long long Conf::GetLL(const std::string &strFile, const std::string &strSection, const std::string &strKey, long long llDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToLL(strConf, llDef);
    }
    return llDef;
}
unsigned long long Conf::GetULL(const std::string &strFile, const std::string &strSection, const std::string &strKey, unsigned long long ullDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToULL(strConf, ullDef);
    }
    return ullDef;
}
float Conf::GetFloat(const std::string &strFile, const std::string &strSection, const std::string &strKey, float fDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToFloat(strConf, fDef);
    }
    return fDef;
}
double Conf::GetDouble(const std::string &strFile, const std::string &strSection, const std::string &strKey, double dDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToDouble(strConf, dDef);
    }
    return dDef;
}
long double Conf::GetLDouble(const std::string &strFile, const std::string &strSection, const std::string &strKey, long double ldDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	Convert::StrToLDouble(strConf, ldDef);
    }
    return ldDef;    
}
std::string Conf::GetString(const std::string &strFile, const std::string &strSection, const std::string &strKey, const std::string &strDef)
{
    std::string strConf;
    if (Conf::GetConfStr(strFile, strSection, strKey, strConf))
    {
	return strConf;
    }
    return std::string(strDef);
}

bool Conf::WriteString(const std::string &strFile, const std::string &strSection, const std::string &strKey, const std::string &strVal)
{
    if (strFile.empty()
	|| strSection.empty()
	|| strKey.empty())
    {
	return false;
    }
    //创建文件全路径
    boost::filesystem::path filePath(strFile);
    boost::filesystem::path parentPath = filePath.parent_path();
    if (!parentPath.empty())
    {
	try
	{
	    if (!boost::filesystem::create_directories(parentPath))
	    {
		return false;
	    }
	}
	catch (boost::filesystem::filesystem_error &e)
	{
	    return false;
	}
    }
    std::fstream fs;
    fs.open(strFile, std::ios::in | std::ios::out);
    if (!fs.is_open())
    {
	return false;
    }
    char cBuf[CONF_MAX_LINE + 1] = {0};
    std::string strBuf;
    while (fs.getline(cBuf, CONF_MAX_LINE))
    {
	strBuf = cBuf;
	if (strBuf.length() == (strSection.length() + 2)
	    && '[' == strBuf.at(0)
	    && ']' == strBuf.at(strSection.length() + 1)
	    && !strBuf.compare(1, strSection.length(), strSection))
	{
	    std::streampos sPos = fs.tellg();
	    while (fs.getline(cBuf, CONF_MAX_LINE))
	    {
		strBuf = cBuf;
		if (0 == strBuf.length()
		    || '[' == strBuf.at(0))
		{
		    break;
		}
		if (strBuf.length() > strKey.length()
		    && '=' == strBuf.at(strKey.length())
		    && !strBuf.compare(0, strKey.length(), strKey))
		{
		    //当包含section和key时
		    fs.seekg(sPos, std::ios::beg);
		    std::string strLastBuf(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());//读取文件最后的内容
		    fs.seekg(sPos, std::ios::beg);
		    fs << strKey << "=" << strVal << "\n";
		    //fs << strLastBuf;
		    return true;
		}
		sPos = fs.tellg();
	    }
	    //当包含section,不包含key时
	    fs.seekg(sPos, std::ios::beg);
	    std::string strLastBuf(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());//读取文件最后的内容
	    fs.seekg(sPos, std::ios::beg);
	    fs << strKey << "=" << strVal << "\n";
	    // fs << strLastBuf;
	    return true;
	}
    }
    //当不包含section和key时
    fs << "[" << strSection << "]\n";
    fs << strKey << "=" << strVal << "\n";
    fs.close();
    return true;
}
bool Conf::WriteBool(const std::string &strFile, const std::string &strSection, const std::string &strKey, bool bVal)
{
    return WriteString(strFile, strSection, strKey, Convert::BoolToStr(bVal));
}
bool Conf::WriteInt(const std::string &strFile, const std::string &strSection, const std::string &strKey, int nVal)
{
    return WriteString(strFile, strSection, strKey, Convert::IntToStr(nVal));
}
bool Conf::WriteUInt(const std::string &strFile, const std::string &strSection, const std::string &strKey, unsigned uVal)
{
    return WriteString(strFile, strSection, strKey, Convert::UIntToStr(uVal));
}
bool Conf::WriteLong(const std::string &strFile, const std::string &strSection, const std::string &strKey, long lVal)
{
    return WriteString(strFile, strSection, strKey, Convert::LongToStr(lVal));
}
bool Conf::WriteULong(const std::string &strFile, const std::string &strSection, const std::string &strKey, unsigned long ulVal)
{
    return WriteString(strFile, strSection, strKey, Convert::ULongToStr(ulVal));
}
bool Conf::WriteLL(const std::string &strFile, const std::string &strSection, const std::string &strKey, long long llVal)
{
    return WriteString(strFile, strSection, strKey, Convert::LLToStr(llVal));
}
bool Conf::WriteFloat(const std::string &strFile, const std::string &strSection, const std::string &strKey, float fVal)
{
    return WriteString(strFile, strSection, strKey, Convert::FloatToStr(fVal));
}
bool Conf::WriteDouble(const std::string &strFile, const std::string &strSection, const std::string &strKey, double dVal)
{
    return WriteString(strFile, strSection, strKey, Convert::DoubleToStr(dVal));
}
bool Conf::WriteLDouble(const std::string &strFile, const std::string &strSection, const std::string &strKey, long double ldVal)
{
    return WriteString(strFile, strSection, strKey, Convert::LDoubleToStr(ldVal));
}
