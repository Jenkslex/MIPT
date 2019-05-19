//!{=================================================================================
//! @file    Lexical_analyzer.h
//! @date    2014-29-11 22:14
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement of lexical analyzer for compile.
//!}=================================================================================
#pragma once


//!----------------------------------------------------------------------------------
//! Includings.
//!----------------------------------------------------------------------------------
#include "Data.h"
#include "Enums.h"


//!----------------------------------------------------------------------------------
//! Lexical_Analyzer class.
//!----------------------------------------------------------------------------------
class Lexical_Analyzer
{
		int Str_pointer_;
		int Lex_pointer_;

	public:
		//! Primary settings
		Lexical_Analyzer ();
		~Lexical_Analyzer ();

		// Analyzer
		Data_t** Lexical_Analysis (char Buff[]);

	private:
		//! Cases for Lexical Analyzer
		void GetOperation (Data_t* Data[], char Buff[]);
		void GetBracket   (Data_t* Data[], char Buff[]);
		void GetNumber	  (Data_t* Data[], char Buff[]);
		void GetString    (Data_t* Data[], char Buff[]);
};

bool isoperator (char symbol);
bool isbracket  (char symbol);

void SyntaxError (char String[], int LexStart, char* description);

