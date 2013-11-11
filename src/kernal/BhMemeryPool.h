#ifndef _BHMEMERYPOOL_H_
#define _BHMEMERYPOOL_H_

class BhMemeryPool
{
public:
    BhMemeryPool();
    explicit BhMemeryPool(unsigned uBlockLen);
    ~BhMemeryPool();
    BhMemeryPool(const BhMemeryPool &obj);
    BhMemeryPool& operator=(const BhMemeryPool &obj);
public:
    bool Init(unsigned uBlockLen);
    void Clear();
    unsigned Length() const;
    bool Read(void *pBuf, unsigned uLen);
    void Write(const void *pBuf, unsigned uLen);
    void Free(unsigned uLen);
private:
    unsigned m_uBlockLen;//每个分块长度
    unsigned m_uBeginPos;//当前位于第一分块的位置
    unsigned m_uEndPos;//当前位于最后一个分块的位置
    unsigned m_uLength;//总长度
    std::list<void *> m_blockList;
};

#endif
