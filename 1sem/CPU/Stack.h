//!{=================================================================================
//! @file    Stack.h
//! @date    2014-10-01 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This programm realise Stack.
//!}=================================================================================


//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <malloc.h>
#include "stkerrno.h"


//!----------------------------------------------------------------------------------
//! Defines.
//!----------------------------------------------------------------------------------
#define CONDITION(condition, ERRNUM)   if (!condition) { stkerrno = ERRNUM; return 0;}


//!----------------------------------------------------------------------------------
//! Constants
//!----------------------------------------------------------------------------------
const int MAX_SIZE = 10;


//!----------------------------------------------------------------------------------
//! Structs
//!----------------------------------------------------------------------------------
struct stack_t
{
    double* Data;
    int top;
    int max_size;
};


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Primary settings
int Stack_ctor (stack_t* stk);
int Stack_dtor (stack_t* stk);

//! Primary operations
int Stack_push (stack_t* stk, double value);
int Stack_pop  (stack_t* stk, double* value);

//! Diagnostics
int  Stack_OK   (const stack_t* stk);
int  Stack_Dump (const stack_t* stk);

//! Help
int Stack_Full  (const stack_t* stk);
int Stack_Empty (const stack_t* stk);


//! Primary settings
//! @{
//===========================================================================
int Stack_ctor (stack_t* stk)
{
    assert (stk != NULL);

    stk->Data = (double*) calloc (MAX_SIZE, sizeof (*(stk->Data)));
    assert (stk->Data != NULL);

    stk->top = 0;
    stk->max_size = MAX_SIZE;

    assert (Stack_OK (stk));

    return 1;
}
//---------------------------------------------------------------------------
int Stack_dtor (stack_t* stk)
{
    assert (Stack_OK (stk));

    free (stk->Data);
    stk->Data = NULL;
    stk->top = -1;
    stk->max_size = -1;

    return 1;
}
//===========================================================================
//! @}


//! Primary operations
//! @{
//===========================================================================
int Stack_push (stack_t* stk, double value)
{
    assert (Stack_OK (stk));
    CONDITION (!Stack_Full (stk),  STKERRFULL)

    stk->Data[stk->top] = value;
    stk->top++;

    assert (Stack_OK (stk));

    return 1;
}
//---------------------------------------------------------------------------
int Stack_pop  (stack_t* stk, double* value)
{
    assert (Stack_OK (stk));
    CONDITION (!Stack_Empty (stk), STKERREMPTY);
    assert (value != NULL);

    stk->top--;
    *value = *(stk->Data + stk->top);

    assert (Stack_OK (stk));

    return 1;
}
//===========================================================================
//! @}


//! Diagnostics
//! @{
//===========================================================================
int Stack_OK   (const stack_t* stk)
{
    CONDITION (stk != NULL,       STKERRNULL);
    CONDITION (stk->Data != NULL, STKERRDATA);

    CONDITION (stk->max_size > 0, STKERRSIZE);
    CONDITION ((stk->top >= 0) && (stk->top <= stk->max_size),
                                  STKERRCOUNT);

    return 1;
}
//---------------------------------------------------------------------------
int Stack_Dump (const stack_t* stk)
{
    printf ("\nStack_t (%d) [0x%p]\n", errno, stk);
    printf ("\tcount = %d\n", stk->top);
    printf ("\tData[%d]  [0x%p]\n", stk->max_size, stk->Data);
    for (int i = 0; i < stk->max_size; i++)
    {
        printf ("\t\t[%d] = %lg",i ,stk->Data[i]);
        if (i >= stk->top) printf ("*");
        printf ("\n");
    }

    return 1;
}
//===========================================================================
//! @}


//! Help
//! @{
//===========================================================================
int Stack_Full (const stack_t* stk)
{
    CONDITION (Stack_OK (stk),     stkerrno);

    if (stk->top == stk->max_size) return 1;
    return 0;
}
//---------------------------------------------------------------------------
int Stack_Empty (const stack_t* stk)
{
    CONDITION (Stack_OK (stk),     stkerrno);

    if (stk->top == 0) return 1;
    return 0;
}
//===========================================================================
//! @}
