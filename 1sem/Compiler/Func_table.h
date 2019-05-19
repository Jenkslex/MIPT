//!{=================================================================================
//! @file    Func_table.h
//! @date    2015-27-03 14:11
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement Data for compile.
//!}=================================================================================
#pragma once


//!----------------------------------------------------------------------------------
//! Includings.
//!----------------------------------------------------------------------------------
#include "Enums.h"


//!----------------------------------------------------------------------------------
//! Constants
//!----------------------------------------------------------------------------------
const int MAX_FUNC_NUM      = 10;
const int MAX_FUNC_NAME_LEN = 20;
const int MAX_FUNC_ARG_NUM  = 10;



//!----------------------------------------------------------------------------------
//! Func_table class.
//!----------------------------------------------------------------------------------
class Func_table_t
{
		string Func_name_;
		int    Arg_num_;

		arg_type* arg_types_;
		string*   arg_names_;

	public:
		//! Primary settings
		Func_table_t (string Name);
		~Func_table_t ();
		
		//! Working with
		void Add_Arg (arg_type argument_type, string argument_name);
		string get_name () const;
		

		//! Diagnostics
		void Export  (ostream& stream) const;
};


ostream& operator << (ostream& stream, const Func_table_t& obj);
