#include "Data.h"
#include "stdafx.h"

//! Data
//! @{
//!==================================================================================
Data_t* new_Data  (info_t type, int    instuction)
{
	assert (type == INSTRUCTION);

	Data_t* Data = (Data_t*) calloc (1, sizeof (Data_t));                             assert (Data);

	Data->type = INSTRUCTION;
	Data->information.instruction = instuction;

	return Data;
}
//!----------------------------------------------------------------------------------
Data_t* new_Data  (info_t type, double number    )
{
	assert (type == NUMBER);

	Data_t* Data = (Data_t*) calloc (1, sizeof (Data_t));                             assert (Data);

	Data->type = NUMBER;
	Data->information.number = number;

	return Data;
}
//!----------------------------------------------------------------------------------
Data_t* new_Data  (info_t type, char*  String    )
{
	assert (type == VARIABLE);

	Data_t* Data = (Data_t*) calloc (1, sizeof (Data_t));                            assert (Data);

	Data->type = VARIABLE;
	Data->information.String = (char*) calloc (MAX_WORD_SIZE, sizeof (char));        assert (Data->information.String);
	strcpy (Data->information.String, String);

	return Data;
}
//!----------------------------------------------------------------------------------
int     Data_dtor (Data_t* Data)
{
	assert (Data != NULL);

	if (Data->type == VARIABLE) free (Data->information.String);
	free (Data);

	return 1;
}
//!----------------------------------------------------------------------------------
Data_t* Data_copy  (Data_t* Data)
{
	assert (Data != NULL);

	Data_t* copy_Data = (Data_t*) calloc (1, sizeof (Data_t));                        assert (copy_Data);

	if (typeof_Data (Data) == INSTRUCTION) 
		{ copy_Data = new_Data (INSTRUCTION, Data->information.instruction);          assert (copy_Data); }

	if (typeof_Data (Data) == NUMBER     ) 
	    { copy_Data = new_Data (NUMBER     , Data->information.number     );          assert (copy_Data); }

	if (typeof_Data (Data) == VARIABLE   ) 
	{
		char* VarName = (char*) calloc (MAX_WORD_SIZE, sizeof (char));                assert (VarName);              
		strcpy (VarName, Data->information.String);
		copy_Data = new_Data (VARIABLE, VarName);                                     assert (copy_Data);
	}

	return copy_Data;
}
//!==================================================================================
//! @}


//! Help
//! @{
//!==================================================================================
info_t  typeof_Data      (Data_t* Data)
{
    assert (Data != NULL);

    return Data->type;
}
//!----------------------------------------------------------------------------------
int     Data_instruction (Data_t* Data)
{
	assert (Data != NULL);
	assert (typeof_Data (Data) == INSTRUCTION);

	return Data->information.instruction;
}
//!----------------------------------------------------------------------------------
double  Data_number      (Data_t* Data)
{
	assert (Data != NULL);
	assert (typeof_Data (Data) == NUMBER);

	return Data->information.number;
}
//!----------------------------------------------------------------------------------
char*   Data_String      (Data_t* Data)
{
	assert (Data != NULL);
	assert (typeof_Data (Data) == VARIABLE);

	return Data->information.String;
}
//!----------------------------------------------------------------------------------
int     isFunction       (int instruction)
{
	if ((instruction == 60) || (instruction == 61) ||
		(instruction == 62) || (instruction == 63) ||
		(instruction == 64) || (instruction == 65))
			return 1;

	return 0;
}
//!----------------------------------------------------------------------------------
int     isOperation      (int instruction)
{
	if ((instruction == 0) || (instruction == 1) ||
		(instruction == 2) || (instruction == 3) ||
		(instruction == 6))
			return 1;

	return 0;
}
//!==================================================================================
//! @}


//! Print
//!==================================================================================
int PrintData (Data_t* Data)
{
    assert (Data != NULL);

	if (Data->type == INSTRUCTION) printf ("instruction %d" , Data->information.instruction);
	if (Data->type == NUMBER     ) printf ("number      %lg", Data->information.number     );
	if (Data->type == VARIABLE   ) printf ("variable    %s" , Data->information.String     );

    return 1;
}
//!==================================================================================
