#pragma once
//!----------------------------------------------------------------------------------
//! Enums.
//!----------------------------------------------------------------------------------
enum cmd_t
{
	#define DEF_CMD_(NAME, NUMBER, TYPE, BUILDCODE, GENERATECODE)  NAME = NUMBER,
    #include "commands.h"
    #undef DEF_CMD_

	CMD_MAX
};
//!----------------------------------------------------------------------------------
enum arg_type_t
{
	NONE = 0,
	NUM  = 1,
	REG  = 2,
	VAR  = 3,
	STK  = 4,
	ADRS = 5
};
//!----------------------------------------------------------------------------------
enum x86_cmd_t
{
	X86_CMD_EAX = 0,
	X86_CMD_EBX = 1,
	X86_CMD_ECX = 2,
	X86_CMD_EDX = 3,
	X86_CMD_ESI = 4,
	X86_CMD_EDI = 5,
	X86_CMD_ESP = 6,
	X86_CMD_EBP = 7,

	#define DEF_X86_CMD_(NAME, NUM, CODE, CALLCODE)  NAME = NUM,
    #include "x86_commands.h"
    #undef DEF_X86_CMD_

	X86_CMD_MAX
};