#ifndef _MSGHANDLER_H_
#define _MSGHANDLER_H_

#include <boost/signals2.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include "../msg/IMsg.h"

class MsgHandler
{
public:
    typedef boost::shared_mutex RWMutex;
    typedef boost::shared_lock<RWMutex> ReadLock;
    typedef boost::unique_lock<RWMutex> WriteLock;
    
    typedef boost::signals2::signal<void (const IMsg *)> SigType;
    typedef boost::ptr_unordered_map<std::string, SigType> SigPunmap;

private:
    MsgHandler() = default;
    ~MsgHandler() = default;
    MsgHandler(const MsgHandler &) = delete;
    MsgHandler& operator=(const MsgHandler &) = delete;
public:
    static MsgHandler& Instance();
    bool Init();
    void Clear();
    void Connect(const std::string &, void (*)(const IMsg *));
    
    template <typename O, typename T>
    void Connect(const std::string &, void (T::*)(const IMsg *), O *);
    
    void Disconnect(const std::string &, void (*)(const IMsg *));
    
    template <typename O, typename T>
    void Disconnect(const std::string &, void (T::*)(const IMsg *), O *);
    
    void Disconnect(const std::string &);
    void DisconnectAll();
    void Invoke(const IMsg *);
private:
    SigPunmap m_sigPunmap;
    RWMutex m_rwMutex;
};
template <typename O, typename T>
void MsgHandler::Connect(const std::string &strClassName, void (T::*pFn)(const IMsg *), O *pObj)
{
    WriteLock writeLock(m_rwMutex);
    m_sigPunmap[strClassName].connect(boost::bind(pFn, pObj, _1));
}
template <typename O, typename T>
void MsgHandler::Disconnect(const std::string &strClassName, void (T::*pFn)(const IMsg *), O *pObj)
{
    ReadLock readLock(m_rwMutex);
    SigPunmap::iterator iter = m_sigPunmap.find(strClassName);
    if (iter != m_sigPunmap.end())
    {
	iter->second->disconnect(boost::bind(pFn, pObj, _1));
    }
}
#endif
