#include "BhMemeryPool.h"
#include <algorithm>

BhMemeryPool::BhMemeryPool(unsigned uBlockLen)
    : m_uBlockLen(uBlockLen)
    , m_uBeginPos(0)
    , m_uEndPos(0)
    , m_uLength(0)
{
}
BhMemeryPool::~BhMemeryPool()
{
    Clear();
}
BhMemeryPool::BhMemeryPool(const BhMemeryPool &obj)
{
    m_uBlockLen = obj.m_uBlockLen;
    m_uBeginPos = obj.m_uBeginPos;
    m_uEndPos = obj.m_uEndPos;
    m_uLength = obj.m_uLength;
    std::list<char *>::const_iterator iter = obj.m_blockList.begin();
    while (iter != obj.m_blockList.end())
    {
	char *pBuf = new char[m_uBlockLen];
	std::copy(*iter, *iter + m_uBlockLen, pBuf);
	m_blockList.push_back(pBuf);
	++iter;
    }
}
BhMemeryPool& BhMemeryPool::operator=(const BhMemeryPool &obj)
{
    if (this == &obj)
    {
	return *this;
    }
    Clear();
    m_uBlockLen = obj.m_uBlockLen;
    m_uBeginPos = obj.m_uBeginPos;
    m_uEndPos = obj.m_uEndPos;
    m_uLength = obj.m_uLength;
    std::list<char *>::const_iterator iter = obj.m_blockList.begin();
    while (iter != obj.m_blockList.end())
    {
	char *pBuf = new char[m_uBlockLen];
	std::copy(*iter, *iter + m_uBlockLen, pBuf);
	m_blockList.push_back(pBuf);
	++ iter;
    }
    return *this;
}
void BhMemeryPool::Clear()
{
    m_uBlockLen = 0;
    m_uBeginPos = 0;
    m_uEndPos = 0;
    m_uLength = 0;
    std::list<char *>::iterator iter = m_blockList.begin();
    while (iter != m_blockList.end())
    {
	delete *iter;
	iter = m_blockList.erase(iter);
    }
}
unsigned BhMemeryPool::Length() const
{
    return m_uLength;
}
bool BhMemeryPool::Read(char *pBuf, unsigned uLen)
{
    if (m_uLength < uLen)
    {
	return false;
    }
    std::list<char *>::iterator iter = m_blockList.begin();
    if (m_uBlockLen - m_uBeginPos >= uLen)
    {
	std::copy(*iter, *iter + uLen, pBuf);
    }
    else
    {
	std::copy(*iter + m_uBeginPos, *iter+ m_uBlockLen, pBuf);
	unsigned uReadLen = m_uBlockLen - m_uBeginPos;
	++ iter;
	while (true)
	{
	    if (uLen - uReadLen > m_uBlockLen)
	    {
		std::copy(*iter, *iter + m_uBlockLen, pBuf + uReadLen);
		uReadLen += m_uBlockLen;
		++ iter;
	    }
	    else
	    {
		std::copy(*iter, *iter + uLen - uReadLen, pBuf + uReadLen);
		break;
	    }
	}
    }
    return true;
}
void BhMemeryPool::Write(const char *pBuf, unsigned uLen)
{
    m_uLength += uLen;
    if (0 != m_uEndPos)
    {
	std::list<char *>::iterator iter = m_blockList.end();
	--iter;
	if (uLen < m_uBlockLen - m_uEndPos)
	{
	    std::copy(pBuf, pBuf + uLen, *iter + m_uEndPos);
	    m_uEndPos += uLen;
	}
	else
	{
	    std::copy(pBuf, pBuf + m_uBlockLen - m_uEndPos, *iter + m_uEndPos);
	    unsigned uWriteLen = m_uBlockLen - m_uEndPos;
	    m_uEndPos = 0;
	    while (true)
	    {
		char *p = new char[m_uBlockLen];
		m_blockList.push_back(p);
		if (uLen - uWriteLen > m_uBlockLen)
		{
		    std::copy(pBuf + uWriteLen, pBuf + uWriteLen + m_uBlockLen, *iter);
		    uWriteLen += m_uBlockLen;
		}
		else
		{
		    std::copy(pBuf + uWriteLen, pBuf + uLen, *iter);
		    m_uEndPos = (uLen - uWriteLen);
		    m_uEndPos %= m_uBlockLen;
		    break;
		}
	    }
	}
    }
    else
    {
	unsigned uWriteLen = 0;
	while (true)
	{
	    char *p = new char[m_uBlockLen];
	    m_blockList.push_back(p);
	    if (uLen - uWriteLen > m_uBlockLen)
	    {
		std::copy(pBuf + uWriteLen, pBuf + uWriteLen + m_uBlockLen, p);
		uWriteLen += m_uBlockLen;
	    }
	    else
	    {
		std::copy(pBuf + uWriteLen, pBuf + uLen, p);
		m_uEndPos = (uLen - uWriteLen);
		m_uEndPos %= m_uBlockLen;
		break;
	    }
	}
    }
}
void BhMemeryPool::Free(unsigned uLen)
{
    if (uLen >= m_uLength)
    {
	Clear();
    }
    else
    {
	m_uLength -= uLen;
	std::list<char *>::iterator iter = m_blockList.begin();
	if (uLen > m_uBlockLen - m_uBeginPos)
	{
	    uLen -= (m_uBlockLen - m_uBeginPos);
	    delete *iter;
	    iter = m_blockList.erase(iter);
	    m_uBeginPos = 0;
	    while (true)
	    {
		if (uLen > m_uBlockLen)
		{
		    delete *iter;
		    iter = m_blockList.erase(iter);
		    uLen -= m_uBlockLen;
		}
		else
		{
		    m_uBeginPos = uLen;
		    if (m_uBeginPos == m_uBlockLen)
		    {
			m_uBeginPos = 0;
			delete *iter;
			iter = m_blockList.erase(iter);
		    }
		}
	    }
	}
	else
	{
	    m_uBeginPos += uLen;
	    if (m_uBeginPos == m_uBlockLen)
	    {
		m_uBeginPos = 0;
		delete *iter;
		iter = m_blockList.erase(iter);
	    }
	}
    }
}
