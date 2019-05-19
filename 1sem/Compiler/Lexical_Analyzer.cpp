#include "stdafx.h"
#include "Lexical_Analyzer.h"


#define RESERVEDNAME(name, num, str)                                                       \
  else if (strcmp (word, str) == 0) Data[Lex_pointer_++] = new Data_t (INSTRUCTION, name); \


//! Primary settings
//! @{
//!==================================================================================
Lexical_Analyzer::Lexical_Analyzer ():
	Str_pointer_ (0),
	Lex_pointer_ (0)
{}
//!----------------------------------------------------------------------------------
Lexical_Analyzer::~Lexical_Analyzer ()
{}
//!==================================================================================
//! @}

//! Analyzer
//!==================================================================================
Data_t** Lexical_Analyzer::Lexical_Analysis (char Buff[])
{
	assert (Buff);

	Data_t** Data = new Data_t* [MAX_LEX_NUM];
	memset   (Data, 0, sizeof (Data_t*) * MAX_LEX_NUM);

	while (Buff[Str_pointer_] != '\0')
	{
		if      (isspace    (Buff[Str_pointer_])) { Str_pointer_++; continue; }

		else if (isoperator (Buff[Str_pointer_])) GetOperation (Data, Buff);
		else if (isbracket  (Buff[Str_pointer_])) GetBracket   (Data, Buff);
		else if (isdigit    (Buff[Str_pointer_])) GetNumber    (Data, Buff);
		else if (isalpha    (Buff[Str_pointer_])) GetString    (Data, Buff);
	}

	Data[Lex_pointer_] = new Data_t (INSTRUCTION, EXIT);

	cout << "Lexical analysis:\n";
	int i = 0;
	while (Data[i]) cout << i << " " << *(Data[i++]) << endl; 


	return Data;
}
//!==================================================================================


//! Cases for Lexical Analyzer
//! @{
//!==================================================================================
void Lexical_Analyzer::GetOperation (Data_t* Data[], char Buff[])
{
	char word[MAX_WORD_SIZE] = {};

	int i = 0;
	while (isoperator (Buff[Str_pointer_])) 
		word[i++] = Buff[Str_pointer_++];
	word[i] = '\0';

	if (0) printf ("LOOL");
	#include "Reserved_names.h"

	else if (strcmp (word, "//") == 0) 
		while (Buff[Str_pointer_] != '\n') Str_pointer_++;
}
//!----------------------------------------------------------------------------------
void Lexical_Analyzer::GetBracket   (Data_t* Data[], char Buff[])
{
	char word[MAX_WORD_SIZE] = {};

	word[0] = Buff[Str_pointer_++];
	word[1] = '\0';

	if (0) printf ("LOOL");
	#include "Reserved_names.h"
}
//!----------------------------------------------------------------------------------
void Lexical_Analyzer::GetNumber    (Data_t* Data[], char Buff[])
{
	char*  NumberEnd = nullptr;
	int value = strtol (Buff + Str_pointer_, &NumberEnd, 0);
	Str_pointer_ = NumberEnd - Buff;

	Data[Lex_pointer_++] = new Data_t (NUMBER, value);
}
//!----------------------------------------------------------------------------------
void Lexical_Analyzer::GetString    (Data_t* Data[], char Buff[])
{
	char word[MAX_WORD_SIZE] = {};

	int i = 0;
	while ((isalpha (Buff[Str_pointer_])) || (isdigit (Buff[Str_pointer_])))
		word[i++] = Buff[Str_pointer_++];
	word[i] = '\0';

	if (0) printf ("LOOL");
	#include "Reserved_names.h"

	else Data[Lex_pointer_++] = new Data_t (OPER_STRING, word);
}
//!==================================================================================
//! @}


//! Help
//!==================================================================================
bool isbracket  (char symbol)
{
	switch (symbol)
	{
		case '(':	case ')':
		case '{':	case '}':
		case '[':	case ']':
			return true;

		default:
			return false;
	}
}
//!----------------------------------------------------------------------------------
bool isoperator (char symbol)
{
	switch (symbol)
	{
		case '+':	case '-':
		case '*':	case '/':
		case '%':	case '=':
		case '<':	case '>':
		case '^':	case ';':
		case '&':	case '|':
		case '!':	case ',':
			return true;

		default:
			return false;
	}
}
//!==================================================================================
#undef RESERVEDNAME
