#include "Convert.h"
#include <iostream>
#include <stdexcept>

std::string Convert::BoolToStr(bool bVal)
{
    return std::to_string(bVal);
}
std::string Convert::IntToStr(int nVal)
{
    return std::to_string(nVal);
}
std::string Convert::UIntToStr(unsigned uVal)
{
    return std::to_string(uVal);
}
std::string Convert::LongToStr(long lVal)
{
    return std::to_string(lVal);
}
std::string Convert::ULongToStr(unsigned long ulVal)
{
    return std::to_string(ulVal);
}
std::string Convert::LLToStr(long long llVal)
{
    return std::to_string(llVal);
}
std::string Convert::ULLToStr(unsigned long long ullVal)
{
    return std::to_string(ullVal);
}
std::string Convert::FloatToStr(float fVal)
{
    return std::to_string(fVal);
}
std::string Convert::DoubleToStr(double dVal)
{
    return std::to_string(dVal);
}
std::string Convert::LDoubleToStr(long double ldVal)
{
    return std::to_string(ldVal);
}
bool Convert::StrToBool(const std::string &strSrc, bool &bDest)
{
    int nTmp = 0;
    if (StrToInt(strSrc, nTmp))
    {
	bDest = (0 != nTmp);
	return true;
    }
    return false;
}
bool Convert::StrToInt(const std::string &strSrc, int &nDest)
{
    try
    {
	nDest = std::stoi(strSrc);
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
bool Convert::StrToUInt(const std::string &strSrc, unsigned &uDest)
{
    try
    {
	uDest = std::stoul(strSrc);//暂时认为long和int相同
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
bool Convert::StrToLong(const std::string &strSrc, long &lDest)
{
    try
    {
	lDest = std::stol(strSrc);
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
bool Convert::StrToULong(const std::string &strSrc, unsigned long &ulDest)
{
    try
    {
	ulDest = std::stoul(strSrc);
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
bool Convert::StrToLL(const std::string &strSrc, long long &llDest)
{
    try
    {
	llDest = std::stoll(strSrc);
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
bool Convert::StrToULL(const std::string &strSrc, unsigned long long &ullDest)
{
    try
    {
	ullDest = std::stoull(strSrc);
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
bool Convert::StrToFloat(const std::string &strSrc, float &fDest)
{
    try
    {
	fDest = std::stof(strSrc);
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
bool Convert::StrToDouble(const std::string &strSrc, double &dDest)
{
    try
    {
	dDest = std::stof(strSrc);
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
bool Convert::StrToLDouble(const std::string &strSrc, long double &ldDest)
{
    try
    {
	ldDest = std::stof(strSrc);
    }
    catch (std::invalid_argument &ex)
    {
	return false;
    }
    catch (std::out_of_range &ex)
    {
	return false;
    }
    return true;
}
