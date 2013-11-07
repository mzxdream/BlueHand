#ifndef _BHMEMERYPOOL_H_
#define _BHMEMERYPOOL_H_

class BhMemeryPool
{
public:
    explicit BhMemeryPool(unsigned uBlockLen);
    ~BhMemeryPool();
    BhMemeryPool(const BhMemeryPool &obj);
    BhMemeryPool& operator=(const BhMemeryPool &obj);
public:
    void Clear();
    unsigned Length() const;
    bool Read(void *pBuf, int nLength);
    void Write(const void *pBuf, int nLength);
    void Free(int nLength);
private:
    unsigned m_uBlockLen;//每个分块长度
    unsigned m_uCurPos;//当前位于第一分块的位置
    unsigned m_uLength;//总长度
    std::list<void *> m_blockList;
};

#endif
