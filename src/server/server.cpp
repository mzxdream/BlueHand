#include "BhServerApp.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    std::string strIP = "";
    int nPort = 3230;
    int nListenCount = 5000;
    int nHandleCount = 20;
    int nBlockLength = 10;
    int nEpollTimeOut = 500;
    int nMsgHeaderLen = 2;
    if (!BhServerApp::Instance().Init(strIP, nPort, nListenCount, nHandleCount, nBlockLength, nEpollTimeOut, nMsgHeaderLen))
    {
	return 0;
    }
    if (!BhServerApp::Instance().Start())
    {
	return 0;
    }
    while (std::cin.get())
    {
	BhServerApp::Instance().Stop();
    }
    BhServerApp::Instance().Wait();
    return 0;
}
