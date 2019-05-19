#ifndef __OBJECT__
#define __OBJECT__

#include <stdio.h>
//#include <windows.h>

class CObject
{
private:
    static FILE*   sDump;
   
public:
    CObject();
    FILE* GetDump();
    
    void Panic();
    void Error(const char* aMessage);
    
    virtual void vCheck() = 0;
    virtual void vDump()  = 0;
    
    void Dump (char     aC);
    void Dump (int      aI);
    void Dump (void*    aP);
    void Dump (float    aF);
    void Dump (CObject& aO); 
    
};

#endif
