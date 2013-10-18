#include "BhThreadPool.h"

BhThreadPool::BhThreadPool(int nCount)
{
    for (int i = 0; i < nCount; ++i)
    {
	m_threadGroup.add_thread(new boost::thread(boost::bind(&BhThreadPool::ThreadMain, this)));
    }
}
BhThreadPool::~BhThreadPool()
{
}
bool BhThreadPool::Init()
{
    return true;
}
void BhThreadPool::Clear()
{
}
void BhThreadPool::Join()
{
    m_threadGroup.join_all();
}
void BhThreadPool::Interrupt()
{
    m_threadGroup.interrupt_all();
}
void BhThreadPool::PushTask(boost::function<void ()> func)
{
    boost::mutex::scoped_lock lock(m_taskMutex);
    m_taskList.push_back(func);
}
void BhThreadPool::ThreadMain()
{
    boost::function<void ()> func;
    bool bGetTask = false;
    while (true)
    {
	{
	    boost::mutex::scoped_lock lock(m_taskMutex);
	    if (m_taskList.empty())
	    {
		bGetTask = false;
	    }
	    else
	    {
		func = m_taskList.front();
		m_taskList.pop_front();
		bGetTask = true;
	    }
	}
	if (bGetTask)
	{
	    func();
	}
	boost::this_thread::interruption_requested();
	boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    }
}
