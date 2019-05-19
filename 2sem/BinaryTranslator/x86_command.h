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


//!----------------------------------------------------------------------------------
//! Includings.
//!----------------------------------------------------------------------------------
const int MAX_CMD_SIZE = 2000;

#define DEF_X86_CMD_(NAME, NUM, CODE, CALLCODE)  const char CODE_##NAME = CODE;
#include "x86_commands.h"
#undef DEF_X86_CMD_


//!----------------------------------------------------------------------------------
//! x86_command class.
//!----------------------------------------------------------------------------------
class x86_command_t
{
		char bin_cmd_[MAX_CMD_SIZE];

		int cmd_size_;

	public:
		x86_command_t ();
		~x86_command_t ();

		#define DEF_X86_CMD_(NAME, NUM, CODE, CALLCODE)  \
			void CALLCODE
		#include "x86_commands.h"
		#undef DEF_X86_CMD_


		char* get_bin_cmd  () { return bin_cmd_;  }
		int   get_cmd_size () { return cmd_size_; }

		void x86_prog_add (x86_command_t* Elem);

		void Export          (ostream& stream) const;
};

ostream&       operator << (ostream& stream, x86_command_t obj);

char getCode (x86_cmd_t reg_num);