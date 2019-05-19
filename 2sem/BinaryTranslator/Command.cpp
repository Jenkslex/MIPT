#include "stdafx.h"
#include "Command.h"


//! Primary settings
//! @{
//!==================================================================================
instr_t::instr_t ():
	cmd_num_ (CMD_EXIT),
	arg_num_ (0),
	
	arg_type_first_  (NONE),
	arg_type_second_ (NONE),

	arg_val_first_  (0),
	arg_val_second_ (0)
{}
//!----------------------------------------------------------------------------------
instr_t::~instr_t ()
{}
//!==================================================================================
//! @}


//! Help
//!==================================================================================
bool instr_t::isJumper ()
{
	switch (cmd_num_)
	{
		case CMD_CALL:	case CMD_JNT:
		case CMD_JMP :
			return true;

		default:
			return false;
	}
}
//!==================================================================================


//! In - Out
//!==================================================================================
void    instr_t::print       (ostream& stream) const
{
	stream << "-----------------------------------------------" << endl;
	stream << "Command ";

	#define DEF_CMD_(NAME, NUMBER, TYPE, BUILDCODE, GENERATECODE) \
		if (cmd_num_ == NAME) cout << TYPE;
	#include "commands.h"
	#undef DEF_CMD_

	stream << endl << "With "  << arg_num_ << " arguments:" << endl; 

	stream << "Argument 1: type: " << arg_type_first_  <<
	                 "    value: " << arg_val_first_   << endl;

	stream << "Argument 2: type: " << arg_type_second_ <<
	                 "    value: " << arg_val_second_  << endl;
	stream << "-----------------------------------------------" << endl;
}
//!==================================================================================


//! Operators
//! @{
//!==================================================================================
ostream& operator << (ostream& stream, const instr_t obj)
{
	obj.print (stream);

	return stream;
};
//!==================================================================================
//! @}