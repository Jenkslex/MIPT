#include "Object.hpp"

FILE* CObject::sDump = 0;

CObject::CObject()
{
    if (!sDump)
    {
        sDump = fopen("dump.txt", "wt");
    }
}
#include <windows.h>
void CObject::Panic()
{
    DebugBreak();
    ///*(int*) 0 = 0; //cross-platform:)
}

void CObject::Dump(char aC)
{
    fprintf(sDump, "%c", aC);
}

void CObject::Dump(int aI)
{
    fprintf(sDump, "%d", aI);
}

void CObject::Dump(void* aP)
{
    fprintf(sDump, "%p", aP);
}

void CObject::Dump(float aF)
{
    fprintf(sDump, "%f", aF);
}

void CObject::Dump(CObject& aO)
{
    aO.vDump();
}

FILE* CObject::GetDump()
{
    return sDump;
}

void CObject::Error(const char *aMessage)
{
    fprintf(GetDump(), "----Error-------------------\n");
    fprintf(GetDump(), "%s\n", aMessage);
    this->vDump();
    fprintf(GetDump(), "----------------------------\n\n\n");
    throw aMessage;
}
