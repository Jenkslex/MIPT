#include "Stream.hpp"

CStream& CStream::operator<<(TByte aData)
{
	vWrite(aData);

	return *this;
}
CStream& CStream::operator<<(TSint aData)
{
	vWrite(aData);

	return *this;
}
CStream& CStream::operator<<(TUint aData)
{
	vWrite(aData);

	return *this;
}
CStream& CStream::operator<<(TReal aData)
{
	vWrite(aData);

	return *this;
}
CStream& CStream::operator>>(TByte& aData)
{
	vRead(aData);

	return *this;
}
CStream& CStream::operator>>(TSint& aData)
{
	vRead(aData);

	return *this;
}
CStream& CStream::operator>>(TUint& aData)
{
	vRead(aData);

	return *this;
}
CStream& CStream::operator>>(TReal& aData)
{
	vRead(aData);

	return *this;
}