#ifndef __FLEEXCEPTION__
#define __FLEEXCEPTION__

#include <stdio.h>
#include <string>

using std::string;

class CFileException
{
private:
    string      mError;
    unsigned    mLineNumber;
    
public:
    CFileException();
    CFileException(const char* aError, FILE* aFile);
    
    void Print();       
};

#endif