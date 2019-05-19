//!{=================================================================================
//! @file    Assembler.h
//! @date    2015-03-03 10:06
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement Assembler for compile.
//!}=================================================================================
#pragma once


//!----------------------------------------------------------------------------------
//! Includings
//!----------------------------------------------------------------------------------
#include "Enums.h"


//!----------------------------------------------------------------------------------
//! Structs
//!----------------------------------------------------------------------------------
struct label_t
{
    char* name;
    int   pos;
};


//!----------------------------------------------------------------------------------
//! Constants
//!----------------------------------------------------------------------------------
const int MAXMARKCOUNT = 10;
const int MAXPROGSIZE  = 400;
const int MAXCMDSIZE   = 30;
const int MAXREGSIZE   = 10;
const int MAXLABELSIZE = 20;
const int MAXVARCOUNT  = 20;
const int MAXVARSIZE   = 10;


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Main function
int Assembler ();

//! Read and Write
int Pass   (int Programm[], int* ProgLen, label_t Labels[], FILE* in);
int SkipComment (FILE* in);
int Record (int Programm[], int  ProgLen, FILE* out);

//! Working with commands
commands_t ChooseCommand (const char* commandstr);

//! Working with labels
int    AddLabel (label_t Labels[], char* commandstr, int ProgLen);
int GetLabel (label_t Labels[], FILE* in);

//! Working with arguments
int scanarg   (FILE* in );
int choosereg (char* reg);

//! Help
bool isWithArgs (commands_t command);
bool isJumper   (commands_t command);

