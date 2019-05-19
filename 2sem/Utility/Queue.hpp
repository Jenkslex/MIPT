#ifndef __QUEUE__
#define __QUEUE__

#include "Object.hpp"

template <typename tType> class CQueue : public CObject
{
private:
	unsigned	mHead;
	unsigned	mTail;
	unsigned	mSize;
	tType*		mData;

public:
	CQueue();
	CQueue(unsigned aSize);
	virtual ~CQueue();

	void		 Push(tType aData);
	tType		 Pop();
	bool		 Full();
	bool		 Empty();  
	virtual void vCheck();
    virtual void vDump();
};

template <typename tType> CQueue<tType>::CQueue(unsigned aSize) :
mHead(0),
mTail(0),
mSize(aSize),
mData(0)
{
	mData = new tType[aSize + 1];
}

template <typename tType> CQueue<tType>::~CQueue()
{
    vCheck();
	delete[] mData;
}

template <typename tType> void CQueue<tType>::Push(tType aData)
{
    vCheck();
	if (Full())
	{
		Error("CQueue::Push: queue is overflowed");
	}
	else
	{
		mData[mHead] = aData;
		mHead = (mHead + 1) % mSize;
	}
}

template <typename tType> tType CQueue<tType>::Pop()
{
    vCheck();
	if (Empty())
	{
		Error("CQueue::Push: queue already empty");
	}
	else	
	{
		tType Returned = mData[mTail];
		mTail = (mTail + 1) % mSize;
		return Returned;
	}
}

template <typename tType> bool CQueue<tType>::Empty()
{
    vCheck();
	return (mTail == mHead);
}

template <typename tType> bool CQueue<tType>::Full()
{
    vCheck();
	return (mHead + 1 == mTail);
}

template <typename tType> void CQueue<tType>::vDump()
{
    fprintf(GetDump(), "CQueue\n{\n");
    fprintf(GetDump(), "\tmHead    = %d\n", mHead);
    fprintf(GetDump(), "\tmTail    = %d\n", mTail);
    fprintf(GetDump(), "\tmSize    = %d\n", mSize);
    fprintf(GetDump(), "\tmData    = %p\n", mData);
    //if (mData)
    //{
    //    for (unsigned i = 0; i < mSize; i++)
    //    {
    //        fprintf(GetDump(), "\t\tmData[&d] = ", i);
    //        Dump(mData[i]);
    //    }
    //}
    fprintf(GetDump(), "}\n");    
}

template<typename tType> void CQueue<tType>::vCheck()
{
    if (!mData)
    {
        fprintf(GetDump(), "----CQueue Dump----------------------\n"); 
        vDump();
        fprintf(GetDump(), "------------------------------------\n\n\n");
        Panic();
    }
}


#endif
