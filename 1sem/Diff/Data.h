//!{=================================================================================
//! @file    Data.h
//! @date    2014-29-11 22:14
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement Data for compile.
//!}=================================================================================


//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include <malloc.h>


//!----------------------------------------------------------------------------------
//! Enums.
//!----------------------------------------------------------------------------------
enum info_t
{
    NEXT        = 0,
    NUMBER      = 1,
    INSTRUCTION = 2,
	VARIABLE    = 3
};


//!----------------------------------------------------------------------------------
//! Structs.
//!----------------------------------------------------------------------------------
union info
{
	int    instruction;
	double number;
	char*  String;  
};
//!----------------------------------------------------------------------------------
struct Data_t
{
    info information;

    info_t  type;
};


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Primary settings
Data_t* new_Data   (info_t type, int    instuction);
Data_t* new_Data   (info_t type, double number    );
Data_t* new_Data   (info_t type, char*  String    );
int     Data_dtor  (Data_t* Data);
Data_t* Data_copy  (Data_t* Data);

//! Help
info_t  typeof_Data      (Data_t* Data);
int     Data_instruction (Data_t* Data);
double  Data_number      (Data_t* Data);
char*   Data_String      (Data_t* Data);
int     isFunction       (int instruction);
int     isOperation      (int instruction);

//! Print
int     PrintData        (Data_t* Data);