//!{=================================================================================
//! @file    List_Elem.h
//! @date    2014-14-11 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This programm implement element of the list.
//!}=================================================================================


//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <malloc.h>
#include <assert.h>
#include <string.h>


//!----------------------------------------------------------------------------------
//! Constants.
//!----------------------------------------------------------------------------------
const int MAX_ELEM_SIZE = 20;


//!----------------------------------------------------------------------------------
//! Structs.
//!----------------------------------------------------------------------------------
typedef char Data_t;

struct ListElem_t
{
    Data_t* Data;
    ListElem_t* prev;
    ListElem_t* next;
};


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Primary settings
int ListElem_ctor (ListElem_t* Elem, Data_t* Data);
int ListElem_dtor (ListElem_t* Elem);

//! Diagnostics
int ListElem_OK   (const ListElem_t* Elem);
int ListElem_Dump (const ListElem_t* Elem, int ElemNumber);

//! PrintData
int PrintData (char* Data);
int PrintData (int*  Data);

//! Compare
int Compare   (ListElem_t* Elem, const char* Data);
int Compare   (ListElem_t* Elem, const int*  Data);

//! TieUp
int TieUp (ListElem_t* Elem_1, ListElem_t* Elem_2);


//! Primary settings
//! @{
//!==================================================================================
int ListElem_ctor (ListElem_t* Elem, Data_t* Data)
{
    assert (Elem != NULL);
    assert (Data != NULL);

    Elem->Data = Data;

    Elem->prev = NULL;
    Elem->next = NULL;

    assert (ListElem_OK (Elem));

    return 1;
}
//!----------------------------------------------------------------------------------
int ListElem_dtor (ListElem_t* Elem)
{
    assert (ListElem_OK (Elem));

    Elem->Data = NULL;
    Elem->prev = NULL;
    Elem->next = NULL;

    return 1;
}
//!==================================================================================
//! @}


//! Diagnostics
//! @{
//!==================================================================================
int ListElem_OK   (const ListElem_t* Elem)
{
    if (Elem == NULL)       return 0;
    if (Elem->Data == NULL) return 0;

    return 1;
}
//!----------------------------------------------------------------------------------
int ListElem_Dump (const ListElem_t* Elem)
{
    printf ("\t[0x%p]\n", Elem);
    printf ("\tprev = 0x%p\n", Elem->prev);
    printf ("\tnext = 0x%p\n", Elem->next);
    printf ("\tData [0x%p]\n", Elem->Data);

    PrintData (Elem->Data);

    return 1;
}
//!==================================================================================
//! @}


//! PrintData
//! @{
//!==================================================================================
int PrintData (char* Data)
{
    printf ("\t%s\n", Data);

    return 1;
}
//!----------------------------------------------------------------------------------
int PrintData (int*  Data)
{
    for (int i = 0; i < MAX_ELEM_SIZE; i++)
        printf ("\tData [0x%p] = %d\n", Data + i, Data[i]);

    return 1;
}
//!==================================================================================
//! @}


//! Compare
//! @{
//!==================================================================================
int Compare   (ListElem_t* Elem, const char* Data)
{
    if (strcmp (Elem->Data, Data) == 0) return 0;

    return 1;
}
//!----------------------------------------------------------------------------------
int Compare   (ListElem_t* Elem, const int*  Data)
{
    for (int i = 0; i < MAX_ELEM_SIZE; i++)
        if (Data[i] != Elem->Data[i]) return 0;

    return 1;
}
//!==================================================================================
//! @}

//! TieUp
//!==================================================================================
int TieUp (ListElem_t* Elem_1, ListElem_t* Elem_2)
{
    assert (ListElem_OK (Elem_1));
    assert (ListElem_OK (Elem_2));

    Elem_1->next = Elem_2;
    Elem_2->prev = Elem_1;

    assert (ListElem_OK (Elem_1));
    assert (ListElem_OK (Elem_2));

    return 1;
}
//!==================================================================================
