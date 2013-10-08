#ifndef _BHSYS_H_
#define _BHSYS_H_

#include <string>

class BhSys
{
public:
    static bool GetAppDirectory(std::string &);
    static bool GetAppFullPath(std::string &);
    static bool GetAppName(std::string &);
};

#endif
