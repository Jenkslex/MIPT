//!{=================================================================================
//! @file    Data.h
//! @date    2015-03-03 10:06
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement Data for compile.
//!}=================================================================================
#pragma once

//!----------------------------------------------------------------------------------
//! Constants.
//!----------------------------------------------------------------------------------
const int MAXCMDNUM   = 100;
const int MAXINSTRNUM = 500;

//!----------------------------------------------------------------------------------
//! Includings.
//!----------------------------------------------------------------------------------
#include "Enums.h"
#include "Command.h"

//!----------------------------------------------------------------------------------
//! Builder class.
//!----------------------------------------------------------------------------------
class Builder
{
		int ProgPointer_;
		int InstrPointer_;
		int CmdListNum_[MAXCMDNUM];

	public:
		//! Primary settings
		Builder ();
		~Builder ();

		//! Builder
		vector<instr_t*>* Build (int* Program);

	private:

		void CalculateOffsets (vector<instr_t*>* List, vector<int> Offsets);

		//! Cases for builder
			#define DEF_CMD_(NAME, NUMBER, TYPE, BUILDCODE, GENERATECODE) \
				instr_t* BUILDCODE (int* Program);
			#include "commands.h"
			#undef  DEF_CMD_
};


//! Help
bool isJumper (cmd_t command);