#ifndef __STREAM__
#define __STREAM__

#include "../Platform/Types.h"

class CStream
{

public:
	virtual void vWrite(TByte aData) = 0;
	virtual void vWrite(TSint aData) = 0;
	virtual void vWrite(TUint aData) = 0;
	virtual void vWrite(TReal aData) = 0;
	virtual void vRead(TByte& aData) = 0;
	virtual void vRead(TSint& aData) = 0;
	virtual void vRead(TUint& aData) = 0;
	virtual void vRead(TReal& aData) = 0;

	CStream& operator<<(TByte  aData);
	CStream& operator<<(TSint  aData);
	CStream& operator<<(TUint  aData);
	CStream& operator<<(TReal  aData);
	CStream& operator>>(TByte& aData);
	CStream& operator>>(TSint& aData);
	CStream& operator>>(TUint& aData);
	CStream& operator>>(TReal& aData);

};

#endif