#include "MsgHandler.h"

MsgHandler& MsgHandler::Instance()
{
    static MsgHandler handler;
    return handler;
}
bool MsgHandler::Init()
{
    return true;
}
void MsgHandler::Clear()
{
    WriteLock writeLock(m_rwMutex);
    m_sigPunmap.clear();
}
void MsgHandler::Connect(const std::string &strClassName, void (*pFn)(const IMsg *))
{
    WriteLock writeLock(m_rwMutex);
    m_sigPunmap[strClassName].connect(pFn);
}
void MsgHandler::Disconnect(const std::string &strClassName, void (*pFn)(const IMsg *))
{
    ReadLock readLock(m_rwMutex);
    SigPunmap::iterator iter = m_sigPunmap.find(strClassName);
    if (iter != m_sigPunmap.end())
    {
	iter->second->disconnect(pFn);
    }
}
void MsgHandler::Disconnect(const std::string &strClassName)
{
    ReadLock readLock(m_rwMutex);
    SigPunmap::iterator iter = m_sigPunmap.find(strClassName);
    if (iter != m_sigPunmap.end())
    {
	iter->second->disconnect_all_slots();
    }
}
void MsgHandler::Invoke(const IMsg *pMsg)
{
    ReadLock readLock(m_rwMutex);
    SigPunmap::iterator iter = m_sigPunmap.find(pMsg->GetClassName());
    if (iter != m_sigPunmap.end())
    {
	(*(iter->second))(pMsg);
    }
}
