#include "stdafx.h"
#include "Data.h"


//! Primary settings
//! @{
//!==================================================================================
Data_t::Data_t (): 
	type_   (BLANK),
	instr_  (),
	number_ (),
	string_ ()
{}
//!----------------------------------------------------------------------------------
Data_t::Data_t (lexem_t type): 
	type_   (type),
	instr_  (),
	number_ (),
	string_ ()
{}
//!----------------------------------------------------------------------------------
Data_t::Data_t (lexem_t type, instr_t instr ):
	type_   (type  ),
	instr_  (instr ),
	number_ (),
	string_ ()
{} 
//!----------------------------------------------------------------------------------
Data_t::Data_t (lexem_t type, int     number):
	type_   (type  ),
	instr_  (),
	number_ (number),
	string_ ()
{}
//!----------------------------------------------------------------------------------
Data_t::Data_t (lexem_t type, string  str   ):
	type_   (type  ),
	instr_  (),
	number_ (),
	string_ ()
{
	this->string_ = str;
}
//!----------------------------------------------------------------------------------
Data_t::~Data_t () {}
//!----------------------------------------------------------------------------------
Data_t* Data_t::copy        () const 
{
	Data_t* NewData = new Data_t;

	NewData -> type_   = type_  ;
	NewData -> instr_  = instr_ ;
	NewData -> number_ = number_;
	NewData -> string_ = string_;

	return NewData;
}
//!==================================================================================
//! @}


//! Help functions
//! @{
//!==================================================================================
lexem_t Data_t::get_type    () const { return type_  ; }
//!----------------------------------------------------------------------------------
instr_t Data_t::get_instr   () const { return instr_ ; }
//!----------------------------------------------------------------------------------
int     Data_t::get_number  () const { return number_; }
//!----------------------------------------------------------------------------------
string  Data_t::get_string  () const { return string_; }
//!----------------------------------------------------------------------------------
bool    Data_t::isOperation () const  
{
	switch (instr_)
	{
		case ADD   :	
		case SUB   :
		case MUL   :	
		case DIV   :
		case ASSIGN:
			return true;

		default: return 0;
	}
}
//!----------------------------------------------------------------------------------
bool    Data_t::isinstrno   (instr_t instr) const
{
	if (type_  != INSTRUCTION) return false;
	if (instr_ != instr      ) return false;

	return true;
}
//!==================================================================================
//! @}


//! In - Out
//!==================================================================================
void    Data_t::print       (ostream& stream) const
{
	switch (type_)
	{
		case INSTRUCTION : stream << "instruction     " << instr_ ; break;
		case NUMBER      : stream << "number          " << number_; break;

		case OPER_STRING : stream << "oper_string     " << string_; break;
		case VARIABLE    : stream << "variable        " << string_; break;
		case NEW_VARIABLE: stream << "new_variable    " << string_; break;
		case ARRAY       : stream << "array           " << string_; break;
		case NEW_ARRAY   : stream << "new_array       " << string_; break;

		case FUNCTION    : stream << "function        " << string_; break;
		case NEW_FUNCTION: stream << "new_function    " << string_; break;
		case PARAMETER   : stream << "parameter       " << string_; break;
		case PARAM_VALUE : stream << "parameter_value " << string_; break;

		case BLANK       : stream << "blank           "           ; break;

		default: assert (0);
	}
}
//!==================================================================================


//! Operators
//! @{
//!==================================================================================
ostream& operator << (ostream& stream, const Data_t obj)
{
	obj.print (stream);

	return stream;
};
//!==================================================================================
//! @}
