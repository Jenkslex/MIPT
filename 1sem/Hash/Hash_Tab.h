//!{=================================================================================
//! @file    Hash_Tab.h
//! @date    2014-15-11 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This programm implement Hash Tablet.
//!}=================================================================================


//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <stdio.h>
#include "List.h"


//!----------------------------------------------------------------------------------
//! Constants.
//!----------------------------------------------------------------------------------
const int MAX_TAB_SIZE = 997;


//!----------------------------------------------------------------------------------
//! Structs.
//!----------------------------------------------------------------------------------
struct HashTab_t
{
    List_t Lists[MAX_TAB_SIZE];
    unsigned int (*HashFunc) (Data_t* Data);
};


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Primary settings
int HashTab_ctor (HashTab_t* HashTab, unsigned int (*HashFunc) (Data_t* Data));
int HashTab_dtor (HashTab_t* HashTab);

//! Diagnostics
int HashTab_OK   (HashTab_t* HashTab);
int HashTab_Dump (HashTab_t* HashTab);

//! Working with tablet
int AddElem      (HashTab_t* HashTab, Data_t* Data);

//! Hash functions
unsigned int HashFunc_1 (Data_t* Data);
unsigned int HashFunc_2 (Data_t* Data);
unsigned int HashFunc_3 (Data_t* Data);
unsigned int HashFunc_4 (Data_t* Data);
unsigned int HashFunc_5 (Data_t* Data);


//! Primary settings
//! @{
//!==================================================================================
int HashTab_ctor (HashTab_t* HashTab, unsigned int (*HashFunc) (Data_t* Data))
{
    assert (HashTab != NULL);

    for (int i = 0; i < MAX_TAB_SIZE; i++)
        List_ctor (&(HashTab->Lists[i]));

    HashTab->HashFunc = HashFunc;

    assert (HashTab_OK (HashTab));

    return 1;
}
//!----------------------------------------------------------------------------------
int HashTab_dtor (HashTab_t* HashTab)
{
    assert (HashTab_OK (HashTab));

    for (int i = 0; i < MAX_TAB_SIZE; i++)
        List_dtor (&(HashTab->Lists[i]));

    HashTab->HashFunc = NULL;

    return 1;
}
//!==================================================================================
//! @}


//! Diagnostics
//! @{
//!==================================================================================
int HashTab_OK   (HashTab_t* HashTab)
{
    for (int i = 0; i < MAX_TAB_SIZE; i++)
        if (!List_OK (&(HashTab->Lists[i]))) return 0;

    if (HashTab->HashFunc == NULL) return 0;

    return 1;
}
//!----------------------------------------------------------------------------------
int HashTab_Dump (HashTab_t* HashTab)
{
    printf ("===========HashTab==Dump==============\n");
    printf ("HashTab [0x%p]\n", HashTab);
    for (int i = 0; i < MAX_TAB_SIZE; i++)
            List_Dump (&(HashTab->Lists[i]));

    printf ("HashFunc [0x%p]\n", HashTab->HashFunc);
    printf ("======================================\n");

    return 1;
}
//!==================================================================================
//! @}


//! Working with tablet
//! @{
//!==================================================================================
int AddElem      (HashTab_t* HashTab, Data_t* Data)
{
    assert (HashTab_OK (HashTab));
    assert (Data != NULL);

    int result = HashTab->HashFunc (Data) % MAX_TAB_SIZE;

    if (Searching (&(HashTab->Lists[result]), Data) == 0)
        AddtoBegin (&(HashTab->Lists[result]), Data);

    assert (HashTab_OK (HashTab));
}
//!==================================================================================
//! @}


//! Hash functions
//! @{
//!==================================================================================
unsigned int HashFunc_1 (Data_t* Data)
{
    return 1;
}
//!----------------------------------------------------------------------------------
unsigned int HashFunc_2 (Data_t* Data)
{
    return (unsigned int) Data[0];
}
//!----------------------------------------------------------------------------------
unsigned int HashFunc_3 (Data_t* Data)
{
    return strlen (Data);
}
//!----------------------------------------------------------------------------------
unsigned int HashFunc_4 (Data_t* Data)
{
    int len = strlen(Data);
    unsigned int sum = 0;

    for (int i = 0; i < len; i++)
        sum += (unsigned int) Data[i];

    return sum;
}
//!----------------------------------------------------------------------------------
unsigned int HashFunc_5 (Data_t* Data)
{
    int length = strlen(Data);
    unsigned int h = 0;

    for (int i = 0; i < length; i++)
        h = (h << 1) ^ (unsigned int) Data[i];

    return h;
}
//!==================================================================================
//! @}
