#include "FileException.hpp"


CFileException::CFileException(const char* aError, FILE* aFile) :
mError(aError),
mLineNumber(1)
{
    int ErrorPosition = ftell(aFile);
    fseek(aFile,SEEK_SET, 0);
    while (ftell(aFile) < ErrorPosition)
    {
        char c = fgetc(aFile);
        if (c == '\n') mLineNumber++;
    }
}

void CFileException::Print()
{
    printf("Error in line %d: %s\n", mLineNumber, mError.c_str());
}
