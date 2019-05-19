#ifndef __ARRAY__
#define __ARRAY__

#include "Stream.hpp"
#include "Object.hpp"

template<typename tType> class CArray
{

private:
	unsigned		mSize;
	tType*			mData;

public:
	CArray();
	CArray(unsigned aSize);
	~CArray();

	void Erase();
	//void Resize();
	bool Empty();
	unsigned Size() {return mSize;}

	tType& operator[](unsigned aIndex);

	friend CStream& operator >>(CStream& aStream, CArray<tType>& aObject);
	friend CStream& operator <<(CStream& aStream, CArray<tType>& aObject);

};

template<typename tType> void CArray<tType>::Erase()
{
	delete[] mData;
	mSize = 0;
}

template<typename tType> tType& CArray<tType>::operator[](unsigned aIndex)
{
	if (aIndex >= mSize) throw "CArray[]: Invalid index";
	else return mData[aIndex];
}

template<typename tType> CArray<tType>::CArray(unsigned aSize) :
mData(new tType[aSize]),
mSize(aSize)
{
}

template<typename tType> CArray<tType>::CArray() :
mData(0),
mSize(0)
{
}

template<typename tType> CArray<tType>::~CArray()
{
	delete[] mData;
}

template<typename tType> CStream& operator >>(CStream& aStream, CArray<tType>& aObject)
{
	aStream >> mSize;
	Erase();
	Resize(mSize);
	for (unsigned i = 0; i < mSize; i++)
	{
		aStream >> mData[i];
	}
}

template<typename tType> CStream& operator <<(CStream& aStream, CArray<tType>& aObject)
{
	aStream << mSize;
	for (unsigned i = 0; i < mSize; i++)
	{
		aStream << mData[i];
	}
}

#endif