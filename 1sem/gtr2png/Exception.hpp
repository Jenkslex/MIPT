#ifndef __EXCEPTION__
#define __EXCEPTION__


#define _CRT_SECURE_NO_WARNINGS

#include <cstdarg>
#include <string>
using std::string;

#define __THROW(__format,...) throw CException(string(__FILE__), __LINE__,string(__FUNCTION__),__format,__VA_ARGS__);

class CException
{
private:
	string		mMessage;
	bool		mAdvancedInfo;

	string		mFile;
	unsigned	mLine;
	string		mFunction;

public:
	CException();
	CException(const char* aFormat, ...);
	CException(string aFile, unsigned aLine, string aFunction, const char* aFormat, ...);

	void Print();
};


#endif