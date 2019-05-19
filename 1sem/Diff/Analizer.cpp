#include "Analizer.h"
#include "stdafx.h"

//! Analizers
//!==================================================================================
Data_t** Lexical_Analysis   (char String[])
{
    assert (String != NULL);

	Data_t** Data = (Data_t**) calloc (MAX_LEX_NUM, sizeof (Data_t*));
	assert (Data != NULL);

	int OpLast = 1,
		Lex_pointer = 0,
        Str_pointer = 0;

    while (String[Str_pointer] != '\0')
    {
        if      (isspace (String[Str_pointer]))  Str_pointer++;

		else if ((OpLast != 1) && (isoperator (String[Str_pointer])))
		{ 
			GetOperation (Data, &Lex_pointer, String, &Str_pointer);
			OpLast = 1; continue;
		}

		else if ((String[Str_pointer] == '(') || (String[Str_pointer] == ')'))
		{
			GetBracket (Data, &Lex_pointer, String, &Str_pointer);
			if (String[Str_pointer - 1] == '(') { OpLast = 1; continue; }
		}

		else if ((isdigit (String[Str_pointer])) || (String[Str_pointer] == '-'))
			GetNumber (Data, &Lex_pointer, String, &Str_pointer);

		else if (isalpha (String[Str_pointer])) 
			GetString (Data, &Lex_pointer, String, &Str_pointer);

		else SyntaxError(String, Str_pointer, "Undefined start of lexem");

		OpLast = 0;
    }

	Data[Lex_pointer] = new_Data (INSTRUCTION, 13);

	return Data;
}
//!==================================================================================
node_t*  Syntactic_Analysis (Data_t** Data)
{
	node_t* root = GetG0 (Data);

	return root;
}
//!==================================================================================


//! Cases for Lexical Analyzer
//! @{
//!==================================================================================
int GetOperation (Data_t* Data[], int* Lex_pointer, char String[], int* Str_pointer)
{
	assert (Data       );
	assert (Lex_pointer);
	assert (String     );
    assert (Str_pointer);

	int LexStart = *Str_pointer;

	char* word = (char*) calloc (MAX_WORD_SIZE, sizeof (char));                         assert (word);

	int i = 0;
	while (isoperator (String[*Str_pointer]))
    {
		if ((i != 0) && (String[*Str_pointer] == '-') && (isdigit (String[(*Str_pointer) + 1]))) break; 
        word[i++] = String[(*Str_pointer)++];
    }
    word[i] = '\0';

	if      (strcmp (word, "+") == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 0); assert (Data[*Lex_pointer]); }
	else if (strcmp (word, "-") == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 1); assert (Data[*Lex_pointer]); } 
	else if (strcmp (word, "*") == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 2); assert (Data[*Lex_pointer]); } 
	else if (strcmp (word, "/") == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 3); assert (Data[*Lex_pointer]); }
	else if (strcmp (word, "^") == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 6); assert (Data[*Lex_pointer]); }

	else SyntaxError(String, LexStart, "Undefined operation"); 

	(*Lex_pointer)++;

	return 1;
}
//!----------------------------------------------------------------------------------
int GetBracket   (Data_t* Data[], int* Lex_pointer, char String[], int* Str_pointer)
{
	assert (Data       );
	assert (Lex_pointer);
	assert (String     );
    assert (Str_pointer);

	char bracket = String[*Str_pointer];
	(*Str_pointer)++;

	if (bracket == '(') { Data[*Lex_pointer] = new_Data (INSTRUCTION, 20);            assert (Data[*Lex_pointer]); }
	if (bracket == ')') { Data[*Lex_pointer] = new_Data (INSTRUCTION, 21);            assert (Data[*Lex_pointer]); }

	(*Lex_pointer)++;

	return 1;
}
//!----------------------------------------------------------------------------------
int GetNumber    (Data_t* Data[], int* Lex_pointer, char String[], int* Str_pointer)
{
	assert (Data       );
	assert (Lex_pointer);
	assert (String     );
    assert (Str_pointer);

	char* NumberEnd = NULL;

	double value = strtod (String + *Str_pointer, &NumberEnd);

	*Str_pointer = NumberEnd - String;

    Data[*Lex_pointer] = new_Data (NUMBER, value);                                    assert (Data[*Lex_pointer]);
    (*Lex_pointer)++;

	return 1;
}
//!----------------------------------------------------------------------------------
int GetString    (Data_t* Data[], int* Lex_pointer, char String[], int* Str_pointer)
{
	assert (Data       );
	assert (Lex_pointer);
	assert (String     );
    assert (Str_pointer);

	char* word = (char*) calloc (MAX_WORD_SIZE, sizeof (char));                             assert (word);

    int i = 0;
    while ((isalpha (String[*Str_pointer])) || (isdigit(String[*Str_pointer])))
        word[i++] = String[(*Str_pointer)++];
    word[i] = '\0';
	
	if      (strcmp (word, "sin" ) == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 60); assert (Data[*Lex_pointer]); }
	else if (strcmp (word, "cos" ) == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 61); assert (Data[*Lex_pointer]); }
	else if (strcmp (word, "asin") == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 62); assert (Data[*Lex_pointer]); }
	else if (strcmp (word, "acos") == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 63); assert (Data[*Lex_pointer]); }
	else if (strcmp (word, "ln"  ) == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 64); assert (Data[*Lex_pointer]); }
	else if (strcmp (word, "log" ) == 0) { Data[*Lex_pointer] = new_Data (INSTRUCTION, 65); assert (Data[*Lex_pointer]); }

	else Data[*Lex_pointer] = new_Data (VARIABLE, word);                                    assert (Data[*Lex_pointer]);
	(*Lex_pointer)++;

	return 1;
}
//!==================================================================================
//! @}


//! Syntactic Grammar
//! @{
//!==================================================================================
node_t* GetG0 (Data_t* Data[])
{
    int Lex_pointer = 0;

    return GetE (Data, &Lex_pointer);
}
//!----------------------------------------------------------------------------------
node_t* GetE  (Data_t* Data[], int* Lex_pointer)
{
	node_t* value_node = GetT (Data, Lex_pointer);                                    assert (value_node);

	while ((INSTRUCTIONNO (Data[*Lex_pointer], 0)) ||
		   (INSTRUCTIONNO (Data[*Lex_pointer], 1)))
    {
		Data_t* DataOper = Data[(*Lex_pointer)++];                                    assert (DataOper);

		node_t* temp_value_node = GetT (Data, Lex_pointer);                           assert (temp_value_node);            
		value_node = new_node (DataOper, value_node, temp_value_node);                assert (value_node);
    }
    return value_node;
}
//!----------------------------------------------------------------------------------
node_t* GetT  (Data_t* Data[], int* Lex_pointer)
{
	node_t* value_node = GetD (Data, Lex_pointer);                                    assert (value_node);

	while ((INSTRUCTIONNO (Data[*Lex_pointer], 2)) ||
		   (INSTRUCTIONNO (Data[*Lex_pointer], 3)))
    {
        Data_t* DataOper = Data[*Lex_pointer];                                        assert (DataOper);
		(*Lex_pointer)++;

        node_t* temp_value_node = GetD (Data, Lex_pointer);                           assert (temp_value_node);
        value_node = new_node (DataOper, value_node, temp_value_node);                assert (value_node);
    }

    return value_node;
}
//!----------------------------------------------------------------------------------
node_t* GetD  (Data_t* Data[], int* Lex_pointer)
{
	node_t* value_node = GetP (Data, Lex_pointer);                                    assert (value_node);

	if (INSTRUCTIONNO (Data[*Lex_pointer], 6))
	{
		Data_t* DataDegree = Data[(*Lex_pointer)++];                                  assert (DataDegree);

		node_t* degree_node = GetP (Data, Lex_pointer);                               assert (degree_node);
		value_node = new_node (DataDegree, value_node, degree_node);                  assert (value_node);
	}

	return value_node;
}
//!----------------------------------------------------------------------------------
node_t* GetP  (Data_t* Data[], int* Lex_pointer)
{

    if (INSTRUCTIONNO (Data[*Lex_pointer], 20))
    {
		(*Lex_pointer)++;
		node_t* brackets_node = GetE (Data, Lex_pointer);                             assert (brackets_node);

        if (!INSTRUCTIONNO (Data[*Lex_pointer], 21))
			GrammarError (Data[*Lex_pointer], "Wrong symbol, need to <)>");

        (*Lex_pointer)++;
        return brackets_node;
    }
    else
    {
        node_t* value_node = NULL;

        value_node = GetV (Data, Lex_pointer);
        if (value_node != NULL) return value_node;

        value_node = GetN (Data, Lex_pointer);
        if (value_node != NULL) return value_node;

		value_node = GetF (Data, Lex_pointer);
        if (value_node != NULL) return value_node;

		GrammarError (Data[*Lex_pointer], "Wrong P");

		return NULL;
    }
}
//!----------------------------------------------------------------------------------
node_t* GetV  (Data_t* Data[], int* Lex_pointer)
{
	if (typeof_Data (Data[*Lex_pointer]) != VARIABLE) return NULL;

	node_t* var_node = new_node (Data[*Lex_pointer], NULL, NULL);                     assert (var_node);
	(*Lex_pointer)++;

    return var_node;
}
//!----------------------------------------------------------------------------------
node_t* GetN  (Data_t* Data[], int* Lex_pointer)
{
	if (typeof_Data (Data[*Lex_pointer]) != NUMBER) return NULL;

	node_t* value_node = new_node (Data[*Lex_pointer], NULL, NULL);                   assert (value_node);
	(*Lex_pointer)++;

    return value_node;
}
//!----------------------------------------------------------------------------------
node_t* GetF  (Data_t* Data[], int* Lex_pointer)
{
	if (typeof_Data (Data[*Lex_pointer]) != INSTRUCTION) return NULL;

	Data_t* FuncData = Data[*Lex_pointer];                                            assert (FuncData);
	(*Lex_pointer)++;

	node_t* arg_node = GetP (Data, Lex_pointer);                                      assert (arg_node);
	node_t* Func_node = new_node (FuncData, arg_node, NULL);                          assert (Func_node);

	return Func_node;
}
//!==================================================================================
//! @}


//! Help
//!==================================================================================
int isoperator (char symbol)
{
	if ((symbol == '+') || (symbol == '-') ||
		(symbol == '*') || (symbol == '/') ||
		(symbol == '%') || (symbol == '=') ||
		(symbol == '<') || (symbol == '>') ||
		(symbol == '^')) return 1;

	return 0;
}
//!==================================================================================

//! Working with errors
//!==================================================================================
int SyntaxError (char String[], int LexStart, char* description)
{
	printf ("Syntax Error\n");
	printf ("Lexem starts on place %d, symbol %c\n", LexStart, String[LexStart]);
	printf ("%s\n", description);

	_getch();
	exit (1);
}
//!----------------------------------------------------------------------------------
int GrammarError (Data_t* Data, char* description)
{
	printf ("Grammar Error\n");
	printf ("Wrong lexem ");
	PrintData (Data);
	printf ("\n");

	printf ("%s\n", description); 

	return 1;
}
//!==================================================================================