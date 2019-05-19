int stkerrno = 0;

int stkstrerr (char* description);
int stkperror (char* String);

enum StkErrors_t
{
#define DEF_ERROR(name,num,descr) STK ## name = num,

#include "errors.h"
STKERRUNKN

#undef DEF_ERROR
};

const char* stkstrerr (int err)
{
#define DEF_ERROR(name,num,descr) if (err == num) return descr;

#include "errors.h"

#undef DEF_ERROR
}

int stkperror (const char* String)
{
    printf ("%s: %s", String, stkstrerr (stkerrno));
    return 1;
}
