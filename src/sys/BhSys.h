#ifndef _BHSYS_H_
#define _BHSYS_H_

#include <string>

//#define _SYS_WIN_
#define _SYS_LINUX_

class BhSys
{
public:
    static bool GetAppDirectory(std::string &);
    static bool GetAppFullPath(std::string &);
    static bool GetAppName(std::string &);
};

#endif
