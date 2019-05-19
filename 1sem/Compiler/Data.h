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


//!----------------------------------------------------------------------------------
//! Data class.
//!----------------------------------------------------------------------------------
class Data_t
{	
		lexem_t type_;

		struct
		{
			instr_t instr_ ;
			int     number_;
			string  string_;
		};

	public:
		//! Primary settings
		Data_t ();
		Data_t (lexem_t type);
		Data_t (lexem_t type, instr_t instr );
		Data_t (lexem_t type, int     number);
		Data_t (lexem_t type, string  str   );

		~Data_t ();

		Data_t* copy () const;


		//! Help functions
		lexem_t get_type   () const;
		instr_t get_instr  () const;
		int     get_number () const;
		string  get_string () const;

		bool isOperation   ()              const;
		bool isinstrno     (instr_t instr) const;


		//! In - Out
		void print         (ostream& stream) const;
};


//! Operators
ostream& operator << (ostream& stream, Data_t obj);