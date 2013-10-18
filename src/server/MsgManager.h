#ifndef _MSGMANAGER_H_
#define _MSGMANAGER_H_

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <list>

struct tagMsgParam
{
    int nSock;
    int nEpoll;
    void (*pFunc)(int, int);
}MsgParam;

class MsgManager
{
private:
    MsgManager();
    ~MsgManager() = default;
    MsgManager(const MsgManager &) = delete;
    MsgManager& operator=(const MsgManager &) = delete;
public:
    MsgManager& Instance();
    bool Init(int nEpoll);
    void Clear();
    bool Start();
    bool Stop();
    bool Wait();
    bool ThreadCount(int);
    int ThreadCount() const;
    void Exec(const MsgParam &);
private:
    void MainProcess();
private:
    boost::ptr_list<boost::thread> m_threadList;
    std::list<MsgParam> m_msgList;
    volatile int m_nThreadCount;
    volatile int m_nActiveCount;
    volatile bool m_bWantRun;
};

#endif
