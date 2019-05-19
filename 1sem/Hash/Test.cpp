//!{=================================================================================
//! @file    Test,cpp
//! @date    2014-14-11 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program tests capabilities of the hash table.
//!}=================================================================================


//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <locale.h>
#include "Hash_Tab.h"


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Reading
char* ReadFromFile (char FileName[], int* FileLength);
char* ReadWord     (char* text, char** pointer);

//! Testing
int Testing (char* text, int FileLength, unsigned int (*HashFunc) (Data_t* Data), char OutputFile[]);

//! Recording
int RecordOnFile (char OuputFile[], HashTab_t* HashTab);


//! Main
//!==================================================================================
int main ()
{
    setlocale (LC_ALL, "Russian");

    int FileLength = 0;

    char* text = ReadFromFile ("Text.txt", &FileLength);

    Testing (text, FileLength,  HashFunc_1, "1.csv");
    Testing (text, FileLength,  HashFunc_2, "2.csv");
    Testing (text, FileLength,  HashFunc_3, "3.csv");
    Testing (text, FileLength,  HashFunc_4, "4.csv");
    Testing (text, FileLength,  HashFunc_5, "5.csv");

    return 0;
}
//!==================================================================================


//! Reading
//! @{
//!==================================================================================
char* ReadFromFile (char FileName[], int* FileLength)
{
    FILE* in = fopen (FileName, "r");
    assert (in != NULL);

    *FileLength  = filelength (fileno(in));

    char* text = (char*) calloc (*FileLength, sizeof(char));
    assert (text != NULL);

    fread (text, *FileLength, sizeof (char), in);

    fclose(in);

    return text;
}
//!----------------------------------------------------------------------------------
char* ReadWord (char* text, char** pointer)
{
    assert (text    != NULL);
    assert (pointer != NULL);

    char* word = *pointer;

    if (**pointer == '\0') return NULL;

    while (isalpha (**pointer))
    {
        **pointer = tolower (**pointer);
        (*pointer)++;
    }
    **pointer = '\0';
    (*pointer)++;

    return word;
}
//!==================================================================================
//! @}

//! Testing
//!==================================================================================
int Testing (char* text, int FileLength, unsigned int (*HashFunc) (Data_t* Data), char OutputFile[])
{
    assert (text != NULL);
    assert (HashFunc != NULL);
    assert (OutputFile != NULL);

    HashTab_t HashTab = {};
    HashTab_ctor (&HashTab, HashFunc);

    char* buf = (char*) calloc (FileLength, sizeof(char));
    assert (buf != NULL);

    strcpy (buf, text);

    char** pointer = &(buf);
    char* word = ReadWord (buf, pointer);

    while (word != NULL)
    {
        AddElem (&HashTab, word);
        word = ReadWord (buf, pointer);
    }

    RecordOnFile (OutputFile, &HashTab);

    HashTab_dtor (&HashTab);
    free (buf);

    return 1;
}
//!==================================================================================


//! Recording
//!==================================================================================
int RecordOnFile   (char OuputFile[] ,HashTab_t* HashTab)
{
    FILE* out = fopen (OuputFile, "w");
    assert (out != NULL);

    for (int i = 0; i < MAX_TAB_SIZE; i++)
    {
        fprintf (out, "%d; ", (&(HashTab->Lists[i]))->number);
    }

    return 1;
}
//!==================================================================================
