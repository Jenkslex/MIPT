#include "stdafx.h"
#include "Func_table.h"

//! Primary settings
//! @{
//!==================================================================================
Func_table_t::Func_table_t (string Name):
	Func_name_ (Name),
	Arg_num_   (   0),
	arg_types_ (new arg_type[MAX_FUNC_ARG_NUM]),
	arg_names_ (new string  [MAX_FUNC_ARG_NUM])
{
	memset (arg_types_, 0, sizeof (arg_type) * MAX_FUNC_ARG_NUM);
}
//!----------------------------------------------------------------------------------
Func_table_t::~Func_table_t ()
{
	delete [] arg_types_;
	delete [] arg_names_;
}
//!==================================================================================
//! @}


//! Working with
//! @{
//!==================================================================================
void   Func_table_t::Add_Arg (arg_type argument_type, string argument_name)
{
	arg_types_[Arg_num_] = argument_type;
	arg_names_[Arg_num_] = argument_name;

	Arg_num_++;
}
//!----------------------------------------------------------------------------------
string Func_table_t::get_name () const { return Func_name_; }
//!==================================================================================
//! @}


//! Diagnostics
//! @{
//!==================================================================================
void   Func_table_t::Export  (ostream& stream) const
{
	stream << "Function: " << Func_name_                  << endl;
	stream << "Has "       << Arg_num_    << " argumets:" << endl;

	for (int i = 0; i < Arg_num_; i++)
	{	
		stream << "argument " <<        i + 1  << endl;
		stream << "type: "    << arg_types_[i] << endl;
		stream << "Name: "    << arg_names_[i] << endl;
	}
}
//!==================================================================================
//! @}


//! Operators
//!==================================================================================
ostream& operator << (ostream& stream, const Func_table_t& obj)
{
	obj.Export (stream);

	return stream;
};
//!==================================================================================