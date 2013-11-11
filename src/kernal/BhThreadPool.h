#ifndef _BHTHREADPOOL_H_
#define _BHTHREADPOOL_H_

#include <boost/thread/thread.hpp>
#include <list>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

class BhThreadPool
{
public:
    explicit BhThreadPool(int);
    ~BhThreadPool();
    BhThreadPool(const BhThreadPool &) = delete;
    BhThreadPool& operator=(const BhThreadPool &) = delete;
public:
    void Join();
    void Interrupt();
    void PushTask(boost::function<void ()> func);
private:
    void ThreadMain();
private:
    std::list<boost::function<void ()>> m_taskList;
    boost::thread_group m_threadGroup;
    boost::mutex m_taskMutex;
    boost::condition m_emptyCond;
    bool m_bWantStop;
};

#endif
