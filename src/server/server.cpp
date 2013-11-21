#include "BhServerApp.h"
#include <iostream>
#include <string>


int main(int argc, char *argv[])
{
    std::string strIP = "";
    int nPort = 3230;
    int nListenCount = 5000;
    int nEpollCount = 100;
    int nEpollTimeOut = 100;
    int nThreadCount = 20;
    unsigned  uBlockLen = 10;
    if (!BhServerApp::Instance().Init(strIP, nPort, nListenCount, nEpollCount, nEpollTimeOut, nThreadCount, uBlockLen))
    {
	return 0;
    }
    if (!BhServerApp::Instance().Start())
    {
	return 0;
    }
    std::cin.get();
    BhServerApp::Instance().Stop();
    std::cout << "stop" << std::endl;
    BhServerApp::Instance().Wait();
    std::cout << "wait end" << std::endl;
    return 0;
}
