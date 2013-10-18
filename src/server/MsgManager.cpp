#include "MsgManager.h"

MsgManager::MsgManager()
    :m_nThreadCount(0)
    ,m_bWantRun(true)
    ,m_nActiveCount(0)
    ,m_nEpoll(-1)
{
}
MsgManager& MsgManager::Instance()
{
    static MsgMananger obj;
    return obj;
}
bool MsgManager::Init(int nEpoll)
{
    m_nEpoll = nEpoll;
    return true;
}
void MsgManager::Clear()
{
}
bool MsgManager::Start()
{
}
bool MsgManager::Stop()
{
}
bool MsgManager::Wait()
{
}
bool MsgManager::ThreadCount(int nThreadCount)
{
}
int MsgManager::ThreadCount() const
{
    return m_nThreadCount;
}
void MsgManager::Exec(int nSock, void (*))
{
}
void MsgManager::MainProcess()
{
    ++m_nActiveCount;
    while (m_bWantRun)
    {
	if (m_nActiveCount > m_nThreadCount)
	{
	    break;
	}
	else if ()
    }
    --m_nActiveCoutn;
}
