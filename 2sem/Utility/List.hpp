#ifndef __LIST__
#define __LIST__

#include "Object.hpp"

template<typename tType> class CList : public CObject
{
    
private:    
	struct tElement
	{
		tElement*	Next;
		tElement*	Prev;
		tType		Data;
		
		tElement(){}		
		tElement(tElement* aPrev, tElement* aNext, tType aData) :
		Data(aData),
		Prev(aPrev),
		Next(aNext)
		{
			if (!Prev)
			{
				throw "tElement::tElement: null prev";
			}
			else
			{
				Prev->Next = this;
			}
			if (!Next)
			{
				throw "tElement::tElement: null next";
			}
			else
			{
				Next->Prev = this;
			}
		}
		~tElement()
		{
			if (Prev)
			{
				Prev->Next = Next;
			}
			if (Next)
			{
				Next->Prev = Prev;
			}
		}
			
	};
	
    tElement    mStart; 
    tElement    mEnd;
	
public:
	class tIterator : public CObject
	{
	
	private:
        tElement*   mElement;
        
    public:

		virtual void vCheck(){}
		virtual void vDump(){};
        
        tIterator();
        tIterator(tElement* aElement) : mElement(aElement){}
        tElement* Element() {return mElement;}
        tIterator operator++()
        {
        	if (mElement->Next)
        	{
        		mElement = mElement->Next;
        	}
        	else
        	{
        		Error("tIterator::++: Last element in the sequence");
        	}
        	return *this;
        } 
        tIterator operator--()
        {
        	if (mElement->Prev)
        	{
        		mElement = mElement->Prev;
        	}
        	else
        	{
        		Error("tIterator::--: First element in the sequence");
        	}
        	return *this;
        }
        tIterator operator++(int)
        {
        	tIterator Old = *this;
        	++(*this);
        	return Old;
        }
        tIterator operator--(int)
        {
        	tIterator Old = *this;
        	--(*this);
        	return Old;
        }
        tType& operator()()
        {
        	return mElement->Data;
        }
        bool operator!=(tIterator aOther)
        {
        	return mElement != aOther.mElement;
        }
    };
	
CList()
{
    mStart.Next = &mEnd;  
    mStart.Prev = 0;
    mEnd.Next   = 0; 
    mEnd.Prev   = &mStart;
}

~CList() 
{
	while (First() != End())
	{
		Remove(First());
	}
}

CList(CList& aOther)
{
    mStart.Next = &mEnd;  
    mStart.Prev = 0;
    mEnd.Next   = 0; 
    mEnd.Prev   = &mStart;
	for (CList::tIterator i = aOther.First(); i != aOther.End(); i++)
	{
		Insert(i());
	}
}

CList operator=(CList& aOther)
{
	for (CList::tIterator i = aOther.First(); i != aOther.End(); i++)
	{
		Insert(i());
	}
}

tIterator Find(unsigned aIndex)
{
	tIterator i = First();
	for (unsigned c = 0; c < aIndex; c++) i++;
	return i;
}

tIterator InsertBefore(tIterator aIter, tType aData)
{
	tElement* Next = aIter.Element();
	tElement* Prev = Next->Prev;
	tElement* New  = new tElement(Prev, Next, aData);
	return tIterator(New);
}

tIterator InsertAfter(tIterator aIter, tType aData)
{
	tElement* Prev = aIter.Element();
	tElement* Next = Prev->Next;
	tElement* New  = new tElement(Prev, Next, aData);
	return tIterator(New);
}

tIterator InsertBefore(unsigned aIndex, tType aData)
{
	return InsertBefore(Find(aIndex), aData);
}

tIterator InsertAfter(unsigned aIndex, tType aData)
{
	return InsertAfter(Find(aIndex), aData);
}

tIterator Insert(tType aData)
{
	return InsertAfter(Last(), aData);
}

void Remove(tIterator aIter)
{
	delete aIter.Element();
}

void Remove(unsigned aIndex)
{
	Remove(Find(aIndex));
}

tIterator Start()
{
	return tIterator(&mStart);
}

tIterator End()
{
	return tIterator(&mEnd);
}

tIterator First()
{
	return tIterator(mStart.Next);
}

tIterator Last()
{
	return tIterator(mEnd.Prev);
}

void vDump()
{
    fprintf(GetDump(), "CList\n{\n");
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

void vCheck()
{
    if (!mStart.Next || !mEnd.Prev || mEnd.Next || mStart.Prev)
    {
        fprintf(GetDump(), "----CList Dump----------------------\n"); 
        vDump();
        fprintf(GetDump(), "------------------------------------\n\n\n");
        Panic();
    }
}

};

#endif
