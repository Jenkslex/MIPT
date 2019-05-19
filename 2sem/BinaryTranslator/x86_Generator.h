//!{=================================================================================
//! @file    Data.h
//! @date    2015-03-03 10:06
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement Data for compile.
//!}=================================================================================
#pragma once


//!----------------------------------------------------------------------------------
//! Includings.
//!----------------------------------------------------------------------------------
#include "Enums.h"
#include "Command.h"
#include "x86_command.h"
#include "MyFunctions.h"


//!----------------------------------------------------------------------------------
//! Constants.
//!----------------------------------------------------------------------------------
const int MAX_JMP_NUM = 200;


//!----------------------------------------------------------------------------------
//! x86_Generator class.
//!----------------------------------------------------------------------------------
class x86_Generator
{

		vector<int> Offsets_;
		int JmpCMDPointer_;
		
	public:
		x86_Generator ();
		~x86_Generator ();

		x86_command_t* Generate (vector<instr_t*>* List);

	private:
		void Pass (vector<instr_t*>* List, vector<x86_command_t*>* Program);

		void           ÑalculateOffsets    (vector<instr_t*>*List, vector<x86_command_t*>* Program);
		void           ClearProgram        (vector<instr_t*>*List, vector<x86_command_t*>* Program);
		x86_command_t* ProgramConstructing (vector<instr_t*>*List, vector<x86_command_t*>* Program);


		#define DEF_CMD_(NAME, NUMBER, TYPE, BUILDCODE, GENERATECODE)    \
			x86_command_t* GENERATECODE (int value);
		#include "commands.h"
		#undef DEF_CMD_
};