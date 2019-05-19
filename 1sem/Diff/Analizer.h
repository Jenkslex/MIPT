//!{=================================================================================
//! @file    Analyzer.h
//! @date    2014-29-11 22:14
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement of compile.
//!}=================================================================================


//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> 
#include "Tree.h"


//!----------------------------------------------------------------------------------
//! Definitions.
//!----------------------------------------------------------------------------------
#define INSTRUCTIONNO( Data, num )                                                  \
	((typeof_Data (Data) == INSTRUCTION) && (Data_instruction (Data) == num))

//!----------------------------------------------------------------------------------
//! Constants.
//!----------------------------------------------------------------------------------
const int MAX_LEX_NUM   = 100;
const int MAX_WORD_SIZE = 10;


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Analizers
Data_t** Lexical_Analysis   (char String[]);
node_t*  Syntactic_Analysis (Data_t** Data);


//! Cases for Lexical Analyzer
int GetOperation (Data_t* Data[], int* Lex_pointer, char String[], int* Str_pointer);
int GetBracket   (Data_t* Data[], int* Lex_pointer, char String[], int* Str_pointer);
int GetNumber    (Data_t* Data[], int* Lex_pointer, char String[], int* Str_pointer);
int GetString    (Data_t* Data[], int* Lex_pointer, char String[], int* Str_pointer);

//! Syntactic Grammar
node_t* GetG0 (Data_t* Data[]);
node_t* GetE  (Data_t* Data[], int* Lex_pointer); // + -
node_t* GetT  (Data_t* Data[], int* Lex_pointer); // * /
node_t* GetD  (Data_t* Data[], int* Lex_pointer); // ^
node_t* GetP  (Data_t* Data[], int* Lex_pointer); // () 
node_t* GetV  (Data_t* Data[], int* Lex_pointer); // a, b, c
node_t* GetN  (Data_t* Data[], int* Lex_pointer); // 123
node_t* GetF  (Data_t* Data[], int* Lex_pointer); // func

//! Help
int isoperator (char symbol);

//! Working with errors
int SyntaxError  (char String[], int LexStart, char* description);
int GrammarError (Data_t* Data, char* description);
