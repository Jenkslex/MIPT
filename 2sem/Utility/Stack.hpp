#ifndef __STACK__
#define __STACK__

#include "Object.hpp"

template<typename tType> class CStack : public CObject
{
private:
	tType*			mData;
	unsigned		mSize;
	unsigned        mTop;

public:
	CStack();
	CStack(unsigned aSize);
	virtual ~CStack();

	void Push(tType aData);
	tType Pop();
	tType Top();
	void Clear();
	bool Empty();
	bool Full();
	virtual void vCheck();
    virtual void vDump();

};

template<typename tType> CStack<tType>::CStack(unsigned aSize) :
mData(new tType[aSize]),
mSize(aSize),
mTop(0)
{
}

template<typename tType> CStack<tType>::~CStack()
{
    vCheck();
	delete[] mData;
}

template<typename tType> tType CStack<tType>::Pop()
{           
    vCheck();
	if (!Empty())
	{
	    return mData[--mTop];
	}
	else 
	{
	    Error("CStack::Pop: Stack is already empty");
	}
}

template<typename tType> void CStack<tType>::Push(tType aData)
{
    vCheck();
	if (!Full())
	{
	    mData[mTop++] = aData;
	}
	else
	{
	    Error("CStack::Top: Stack is full");
	}
}

template<typename tType> tType CStack<tType>::Top()
{        
    vCheck();
	if (!Empty())
	{
		return mData[mTop - 1];
	}
	else
	{
		Error("CStack::Top: Stack is empty");
	}
}

template<typename tType> void CStack<tType>::Clear()
{      
    vCheck();
    mTop = 0;
}

template<typename tType> bool CStack<tType>::Full() 
{       
    vCheck();
	return (mTop == mSize);
}

template<typename tType> bool CStack<tType>::Empty()
{      
    vCheck();
	return (mTop == 0);
}

template<typename tType> void CStack<tType>::vDump()
{  
    fprintf(GetDump(), "CStack\n{\n");
    fprintf(GetDump(), "\tmCounter = %d\n", mTop);
    fprintf(GetDump(), "\tmSize    = %d\n", mSize);
    fprintf(GetDump(), "\tmData    = %p\n", mData);
    //if (mData)
    //{
    //   for (unsigned i = 0; i < mSize; i++)
    //    {
    //        fprintf(GetDump(), "\t\tmData[&d] = ", i);
    //        Dump(mData[i]);
    //    }
    //}
    fprintf(GetDump(), "}\n");         
}

template<typename tType> void CStack<tType>::vCheck()
{
    if (mTop > mSize || !mData)
    {
        fprintf(GetDump(), "----CStack Dump----------------------\n"); 
        vDump();
        fprintf(GetDump(), "------------------------------------\n\n\n");
        Panic();
    }
}


#endif
