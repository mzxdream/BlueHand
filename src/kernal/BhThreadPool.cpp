#include "BhThreadPool.h"

BhThreadPool::BhThreadPool(int nCount)
    :m_bWantStop(false);
{
    for (int i = 0; i < nCount; ++i)
    {
	m_threadGroup.add_thread(new boost::thread(boost::bind(&BhThreadPool::ThreadMain, this)));
    }
}
BhThreadPool::~BhThreadPool()
{
}
void BhThreadPool::Join()
{
    m_threadGroup.join_all();
}
void BhThreadPool::Interrupt()
{
    m_bWantStop = true;
    m_emptyCond.notify_all();
}
void BhThreadPool::PushTask(boost::function<void ()> func)
{
    boost::mutex::scoped_lock lock(m_taskMutex);
    m_taskList.push_back(func);
    m_emptyCond.notify_one();
}
void BhThreadPool::ThreadMain()
{
    boost::function<void ()> func;
    while (true)
    {
	boost::mutex::scoped_lock lock(m_taskMutex);
	while (m_taskList.empty())
	{
	    m_emptyCond.wait(lock);
	}
	if (m_bWantStop)
	{
	    return;
	}
	(m_taskList.front())();//执行
	m_taskList.pop_front();
    }
}
