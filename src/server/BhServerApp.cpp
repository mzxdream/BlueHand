#include "BhServerApp.h"

BhServerApp& BhServerApp::Instance()
{
    static BhServerApp instance;
    return instance;
}
bool BhServerApp::Init()
{
    return true;
}
void BhServerApp::Clear()
{
}
void BhServerApp::operator()()
{
    
}
