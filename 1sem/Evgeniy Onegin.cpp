//!{=================================================================================
//! @file    Evgeniy Onegin.cpp
//! @date    2014-10-01 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This programm sort any Novel by lexicografical
//!}=================================================================================

//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <io.h>
#include <cctype>
#include <iostream>

//!----------------------------------------------------------------------------------
//! Defines.
//!----------------------------------------------------------------------------------
#define min(x,y) ((x)<(y))?(x):(y)

//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! User interface.
int Decision ();

//! Work with files
char* read_from_file (int* length);

//! Logic.
int Countof (const char* text, const int length, const char symbol);
int SetStrings (char** Strings, char* text);

//! Print.
int PrintAllStrings (char** Strings, int CountOfLines);
int PrintString (const char* text);

//! Compare functions.
int cmpfunc_start (const void* a, const void* b);
int cmpfunc_end   (const void* a, const void* b);

//! Strcmp from end.
int strcmp_from_end (const char* a, const char* b);


//! Main.
//!==================================================================================
int main ()
{
    setlocale(LC_ALL, "Russian");

    int len = 0;
    char* buf = read_from_file (&len);

    int CountOfLines = Countof (buf, len, '\n') + 1;
    char** Strings = (char**) calloc (CountOfLines, sizeof (char*));

    if (!Strings)
    {
        printf ("Cannot create Array of Strings.\nPlease, reload programm.");
    }

    SetStrings(Strings, buf);

    int answer = Decision ();

    switch (answer)
    {
        case 1:
            PrintAllStrings (Strings, CountOfLines);
            break;
        case 2:
            qsort (Strings, CountOfLines, sizeof (char*), cmpfunc_start);
            PrintAllStrings (Strings, CountOfLines);
            break;
        case 3:
            qsort (Strings, CountOfLines, sizeof (char*), cmpfunc_end);
            PrintAllStrings (Strings, CountOfLines);
            break;
    }

    free (buf);
    buf = NULL;

    free (Strings);
    Strings = NULL;

    return 0;
}
//!==================================================================================


//! User interface.
//!==================================================================================
int Decision ()
{
    int answer = 0;

    printf ("This programm sort lines of Novel.\n");
    printf ("Press 1 to output lines like Pushkin \n");
    printf ("Press 2 to output lines like Belinsky\n");
    printf ("Press 3 to output lines in ryhmes\n");
    printf ("Press any other number to exit programm.\n");

    scanf ("%d", &answer);

    return answer;
}
//!==================================================================================


//! Work with files
//!==================================================================================
char* read_from_file (int* length)
{
    FILE* f = fopen ("Novel.txt", "r");

    if (!f)
    {
        printf ("Cannot open file.\nPlease, reload programm.\n");
        return (char*) "ERROPN";
    }

    *length  = filelength (fileno(f));
    char* text = (char*) calloc (*length, sizeof(char));

    if (!text)
    {
        printf ("Too big file.\nPlease, reload programm.\n");
        exit (1);
    }

    fread (text, *length, sizeof (char), f);

    fclose(f);

    return text;
}
//!==================================================================================


//! Logic
//!==================================================================================
int Countof (const char* text, const int length, const char symbol)
{
    int count = 0;
    for (int i = 0; i <= length; i++)
    {
        if (*text == symbol) count++;
        *text++;
    }
    return count;
}
//!----------------------------------------------------------------------------------
int SetStrings (char** Strings, char* text)
{
    *Strings = text;
    *Strings++;
    *text++;

    while (*text != '\0')
    {
        if(*(text - 1) == '\n')
        {
            *Strings = text;
            *Strings++;
            *(text - 1) = '\0';
        }
        *text++;
    }

    return 0;
}
//!==================================================================================


//! Print.
//!==================================================================================
int PrintAllStrings (char** Strings, int CountOfLines)
{
    for (int i = 0; i < CountOfLines; i++)
    {
        PrintString (Strings[i]);
        printf ("\n");
    }
}
//!---------------------------------------------------------------------------------
int PrintString (const char* text)
{
    while (*text != '\0')
    {
        putchar(*text);
        *text++;
    }
}
//!==================================================================================


//! Compare functions.
//!==================================================================================
int cmpfunc_start (const void* a, const void* b)
{
    return strcmp (*(char**) a, *(char**) b);
}
//!----------------------------------------------------------------------------------
int cmpfunc_end (const void* a, const void* b)
{
    return strcmp_from_end (*(char**) a, *(char**) b);
}
//!==================================================================================


//! Strcmp from end
//!==================================================================================
int strcmp_from_end (const char* a, const char* b)
{
    const char* a_end = a + strlen (a) - 1,
              * b_end = b + strlen (b) - 1;

    while (!isalpha (*a_end) && a_end > a) a_end--;
    while (!isalpha (*b_end) && b_end > b) b_end--;

    while (a_end > a && b_end > b)
    {
        if (*a_end > *b_end) return  1;
        if (*a_end < *b_end) return -1;

        a_end--;
        b_end--;
    }
    return 0;
}
//!==================================================================================
