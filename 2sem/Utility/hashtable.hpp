#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "Object.hpp"
#include "List.hpp"
#include "Array.hpp"

template<typename tType> class CHashTable : public CObject
{
public:
    typedef unsigned        (*tHashFunc)(tType* aData);
    typedef CList<tType>    tCell;
    
private:
    tHashFunc               mHashFunc;
    CArray<tCell>           mTable;
    
public:
    CHashTable();
    virtual void vCheck();
    virtual void vDump();
    
    CHashTable(tHashFunc aHashFunc, unsigned aMaxHash) :
    mTable(aMaxHash),
    mHashFunc(aHashFunc)
    {
    }
    
    unsigned Insert(tType aData)
    {
        unsigned CellIndex = mHashFunc(&aData) % mTable.Size();
        mTable[CellIndex].Insert(aData);
        return CellIndex;
    }
    
    tCell& operator[](unsigned aHash)
    {
        return mTable[aHash % mTable.Size()];
    }
        
    
};

template<typename tType> void CHashTable<tType>::vDump()
{
    fprintf(GetDump(), "CHashTable\n{\n");
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


template<typename tType> void CHashTable<tType>::vCheck()
{
    if (!mHashFunc)
    {
        fprintf(GetDump(), "----CHashTable Dump----------------------\n"); 
        vDump();
        fprintf(GetDump(), "------------------------------------\n\n\n");
        Panic();
    }
}

#endif 