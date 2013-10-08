#include "BhSys.h"
#include "BhSysDefine.h"

#ifdef _SYS_WIN_
#include <windows.h>
#elif define _SYS_LINUX_
#include <limits.h>
#include <unistd.h>
#endif

bool BhSys::GetAppDirectory(std::string &strPath)
{
    if (GetAppFullPath(strPath))
    {
	size_t pos = strPath.find_last_of("/\\");
	if (pos != std::string::npos)
	{
	    strPath = strPath.substr(0, pos+1);
	    return true;
	}
    }
    return false;
}
bool BhSys::GetAppName(std::string &strPath)
{
    if (GetAppFullPath(strPath))
    {
	size_t pos = strPath.find_last_of("/\\");
	if (pos != std::string::npos)
	{
	    strPath = strPath.substr(pos+1);
	}
    }    
    return false;
}
bool BhSys::GetAppFullPath(std::string &strPath)
{
#ifdef _SYS_WIN_
    char szPath[MAX_PATH + 1];
    if (GetModuleFileName(nullptr, szPath, MAX_PATH))
    {
	strPath = szPath;
	return true;
    }
#elif define _SYS_LINUX_
    char szPath[PATH_MAX + 1];
    int nRet = readlink("/proc/self/exe", szPath, PATH_MAX);
    if (nRet >= 0 && nRet <= PATH_MAX)
    {
	szPath[nRet] = '\0';
	strPath = szPath;
	return true;
    }
#endif
    return false;
}
