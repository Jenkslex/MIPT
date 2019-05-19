#include "Exception.hpp"

CException::CException(const char* aFormat, ...) :
mAdvancedInfo(false)
{
	va_list FormatArgs;
	va_start(FormatArgs, aFormat);
	//pretty unsafe
	char Message[1024] = "";
	vsprintf(Message, aFormat, FormatArgs);
	va_end(FormatArgs);
	mMessage = Message;
}

CException::CException(string aFile, unsigned aLine, string aFunction, const char* aFormat, ...) :
mFile(aFile),
mLine(aLine),
mFunction(aFunction),
mAdvancedInfo(true)
{
	va_list FormatArgs;
	va_start(FormatArgs, aFormat);
	//pretty unsafe
	char Message[1024] = "";
	vsprintf(Message, aFormat, FormatArgs);
	va_end(FormatArgs);
	mMessage = Message;
}

void CException::Print()
{
	printf("Error: %s\n", mMessage.c_str());
	if (mAdvancedInfo)
	{
		//printf("\tOccured in [\"%s\":%d (%s)].\n", mFile.c_str(), mLine, mFunction.c_str());
		printf("\tFile:     %s\n", mFile.c_str());
		printf("\tLine:     %d\n", mLine);
		printf("\tFunction: %s\n", mFunction.c_str());
	}
}