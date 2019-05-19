#pragma once
//!----------------------------------------------------------------------------------
//! Enums.
//!----------------------------------------------------------------------------------
enum lexem_t
{
	BLANK        =   0,
	INSTRUCTION  =   1,
	NUMBER       =   2,

	OPER_STRING  =   3,
	VARIABLE     =   4,
	NEW_VARIABLE =   5,
	ARRAY        =   6,
	NEW_ARRAY    =   7,

	NEW_FUNCTION =   8,
	FUNCTION     =   9,
	PARAMETER    =  10,
	PARAM_VALUE  =  11
};
//!----------------------------------------------------------------------------------
enum instr_t
{
	#define RESERVEDNAME(name, num, str)  name = num,
	#include "Reserved_names.h"
	#undef  RESERVEDNAME

	INSTR_MAX
};
//!----------------------------------------------------------------------------------
enum arg_type
{
	ARG_VALUE = 0,
	ARG_ARRAY  = 1
};
//!----------------------------------------------------------------------------------
enum commands_t
{
	#define DEF_CMD_(name, num, args, code)  name = num,
    #include "commands.h"
    #undef DEF_CMD_

    CMD_MAX
};