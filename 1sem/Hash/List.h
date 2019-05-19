//!{=================================================================================
//! @file    List.h
//! @date    2014-14-11 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement list.
//!}=================================================================================


//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <malloc.h>
#include <assert.h>
#include "List_Elem.h"


//!----------------------------------------------------------------------------------
//! Definitions.
//!----------------------------------------------------------------------------------
#define CREATEELEM(Elem, Data) ListElem_t* Elem =                                \
                                (ListElem_t*) calloc (1, sizeof (ListElem_t));   \
                               assert (Elem != NULL);                            \
                               ListElem_ctor (Elem, Data);

#define REMOVEELEM(Elem)       ListElem_dtor (Elem);                             \
                               free (Elem);                                      \
                               Elem = NULL;


//!----------------------------------------------------------------------------------
//! Structs.
//!----------------------------------------------------------------------------------
struct List_t
{
    int number;
    ListElem_t* first;
    ListElem_t* last;
};


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Primary settings
int List_ctor (List_t* List);
int List_dtor (List_t* List);

//! Diagnostics
int List_OK   (const List_t* List);
int List_Dump (const List_t* List);

//! Add
int AddFirst   (List_t* List, Data_t* Data);
int AddRightOf (List_t* List, Data_t* Data, int number);
int AddLeftOf  (List_t* List, Data_t* Data, int number);
int AddBetween (List_t* List, Data_t* Data, int first, int second);
int AddtoBegin (List_t* List, Data_t* Data);
int AddtoEnd   (List_t* List, Data_t* Data);

//! Remove
int RemoveLast      (List_t* List);
int RemoveNumber    (List_t* List, int number);
int RemoveFromEnd   (List_t* List);
int RemoveFromBegin (List_t* List);


//!Help
ListElem_t* GetPointerOnElem (const List_t* List, int number);
int Searching (const List_t* List, const Data_t* Data);


//! Primary settings
//! @{
//!==================================================================================
int List_ctor (List_t* List)
{
    assert (List != NULL);

    List->number = 0;
    List->first  = NULL;
    List->last   = NULL;

    assert (List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int List_dtor (List_t* List)
{
    assert (List_OK (List));

    for (int i = 1; i < List->number; i++) RemoveFromBegin (List);

    List->number = -1;
    List->first  = NULL;
    List->last   = NULL;

    return 1;
}
//!==================================================================================
//! @}


//! Diagnostics
//! @{
//!==================================================================================
int List_OK   (const List_t* List)
{
    if (List == NULL)         return 0;
    if (((List->first == NULL) && (List->number != 0)) ||
        ((List->last  == NULL) && (List->number != 0))) return 0;

    if (List->number != 0)
    {
        ListElem_t* CurrentElem = List->first;
        ListElem_t* PrevElem = NULL;

        for (int i = 1; i < List->number; i++)
        {
            PrevElem = CurrentElem;
            CurrentElem = CurrentElem->next;
            if (PrevElem != CurrentElem->prev) return 0;
        }

        if (CurrentElem != List->last) return 0;
    }

    return 1;
}
//!----------------------------------------------------------------------------------
int List_Dump (const List_t* List)
{
    printf ("-------------LIST--DUMP-------------\n");
    printf ("List [0x%p]\n", List);
    printf ("\tnumber = %d\n",  List->number);
    printf ("\tfirst = 0x%p\n", List->first);
    printf ("\tlast  = 0x%p\n", List->last);

    ListElem_t *Elem = NULL;
    if (List->number != 0)
    {
        Elem = List->first;
        for (int i = 0; i < List->number; i++)
        {
            printf ("Element %d\n", i + 1);
            ListElem_Dump (Elem);
            Elem = Elem->next;
        }
    }
    printf ("-----------------------------------\n");

    return 1;
}
//!==================================================================================
//! @}


//! Add
//! @{
//!==================================================================================
int AddFirst     (List_t* List, Data_t* Data)
{
    assert (List_OK (List));
    assert (Data != NULL);
    assert (List->number == 0);

    CREATEELEM (NewElem, Data)

    List->first = NewElem;
    List->last  = NewElem;

    List->number++;

    assert(List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int AddRightOf   (List_t* List, Data_t* Data, int number)
{
    assert (List_OK (List));
    assert (Data != NULL);
    assert ((number >= 1) && (number <= List->number));

    if (List->number == 0)      { AddFirst   (List, Data);                     return 1; }
    if (List->number <= number) { AddtoEnd   (List, Data);                     return 1; }
    else                        { AddBetween (List, Data, number, number + 1); return 1; }

    assert (List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int AddLeftOf    (List_t* List, Data_t* Data, int number)
{
    assert (List_OK (List));
    assert (Data != NULL);
    assert ((number >= 1) && (number <= List->number));

    if (List->number == 0) { AddFirst   (List, Data);                     return 1; }
    if (number       == 1) { AddtoBegin (List, Data);                     return 1; }
    else                   { AddBetween (List, Data, number - 1, number); return 1; }

    assert (List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int AddBetween   (List_t* List, Data_t* Data, int first, int second)
{
    assert (List_OK (List));
    assert (Data != NULL);
    assert ((first  >= 1) && (first  <= List->number));
    assert ((second >= 1) && (second <= List->number));
    assert (first + 1 == second);

    CREATEELEM (NewElem, Data)

    ListElem_t* ElemBefore = GetPointerOnElem (List, first);
    ListElem_t* ElemAfter  = GetPointerOnElem (List, second);

    assert (ElemBefore != NULL);
    assert (ElemAfter  != NULL);

    TieUp (ElemBefore,  NewElem);
    TieUp (NewElem,     ElemAfter );

    List->number++;

    assert (List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int AddtoBegin   (List_t* List, Data_t* Data)
{
    assert (List_OK (List));
    assert (Data != NULL);

    if (List->number == 0) { AddFirst (List, Data); return 1; }

    CREATEELEM (NewElem, Data)
    ListElem_t* FirstElem = List->first;

    TieUp (NewElem, FirstElem);

    List->first = NewElem;
    List->number++;

    assert (List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int AddtoEnd     (List_t* List, Data_t* Data)
{
    assert (List_OK (List));
    assert (Data != NULL);

    if (List->number == 0) { AddFirst (List, Data); return 1; }

    CREATEELEM (NewElem, Data)
    ListElem_t* LastElem = List->last;

    TieUp (LastElem, NewElem);

    List->last = NewElem;
    List->number++;

    assert (List_OK (List));

    return 1;
}
//!==================================================================================
//! @}


//! Remove
//! @{
//!==================================================================================
int RemoveLast      (List_t* List)
{
    assert (List_OK (List));
    assert (List->number == 1);

    ListElem_t* DelElem = List->first;
    assert (DelElem != NULL);
    REMOVEELEM (DelElem)

    List->first = NULL;
    List->last  = NULL;

    List->number--;

    assert (List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int RemoveNumber    (List_t* List, int number)
{
    assert (List_OK (List));
    assert (List->number > 0);
    assert ((number >= 1) && (number <= List->number));

    if (List->number ==            1) { RemoveLast      (List); return 1; }
    if (number       ==            1) { RemoveFromBegin (List); return 1; }
    if (number       == List->number) { RemoveFromEnd   (List); return 1; }

    ListElem_t* ElemBefore  = GetPointerOnElem (List, number - 1);
    ListElem_t* ElemAfter   = GetPointerOnElem (List, number + 1);

    assert (ElemBefore != NULL);
    assert (ElemAfter  != NULL);

    ListElem_t* DelElem = GetPointerOnElem (List, number);
    assert (DelElem != NULL);
    REMOVEELEM (DelElem)

    TieUp (ElemBefore, ElemAfter);

    List->number--;

    assert (List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int RemoveFromEnd   (List_t* List)
{
    assert (List_OK (List));
    assert (List->number > 0);

    if (List->number == 1) { RemoveLast (List); return 1; }

    ListElem_t* ElemBefore  = GetPointerOnElem (List, List->number - 1);

    ListElem_t* DelElem = GetPointerOnElem (List, List->number);
    assert (DelElem != NULL);
    REMOVEELEM (DelElem)

    ElemBefore->next = NULL;
    List->last = ElemBefore;

    List->number--;

    assert (List_OK (List));

    return 1;
}
//!----------------------------------------------------------------------------------
int RemoveFromBegin (List_t* List)
{
    assert (List_OK (List));
    assert (List->number > 0);

    if (List->number == 1) { RemoveLast (List); return 1; }

    ListElem_t* ElemAfter  = GetPointerOnElem (List, 2);

    ListElem_t* DelElem = List->first;
    assert (DelElem != NULL);
    REMOVEELEM (DelElem)

    ElemAfter->prev = NULL;
    List->first = ElemAfter;

    List->number--;

    assert (List_OK (List));

    return 1;
}
//!==================================================================================
//! @}


//!Help
//!==================================================================================
ListElem_t* GetPointerOnElem (const List_t* List, int number)
{
    assert (List_OK (List));
    assert ((number >= 1) && (number <= List->number));
    assert (List->number != 0);

    ListElem_t* Elem = List->first;
    for (int i = 1; i < number; i++)  Elem = Elem->next;

    assert (List_OK (List));

    return Elem;
}
//!----------------------------------------------------------------------------------
int Searching (const List_t* List, const Data_t* Data)
{
    assert (List_OK (List));
    assert (Data != NULL);

    if (List->number == 0) return 0;

    ListElem_t* Elem = List->first;
    for (int i = 1; i <= List->number; i++)
    {
        if (Compare (Elem, Data) == 0) return i;
        Elem = Elem->next;
    }

    return 0;
}
//!==================================================================================
