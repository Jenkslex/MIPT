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
//! Command class.
//!----------------------------------------------------------------------------------
class instr_t
{
		cmd_t cmd_num_;
		int arg_num_;

		arg_type_t arg_type_first_;
		arg_type_t arg_type_second_;

		int arg_val_first_;
		int arg_val_second_;

	public:
		instr_t ();
		~instr_t ();


		void set_cmd_num (cmd_t cmd_num) { cmd_num_ = cmd_num; }
		void set_arg_num (int   arg_num) { arg_num_ = arg_num; }

		void set_arg_type_fst (arg_type_t arg_type) { arg_type_first_  = arg_type; }
		void set_arg_type_sec (arg_type_t arg_type) { arg_type_second_ = arg_type; }

		void set_arg_val_fst  (int arg_val) { arg_val_first_  = arg_val; }
		void set_arg_val_sec  (int arg_val) { arg_val_second_ = arg_val; }


		cmd_t get_cmd_num () const { return cmd_num_; }
		int   get_arg_num () const { return arg_num_; }

		arg_type_t get_arg_type_fst () const { return arg_type_first_;  }
		arg_type_t get_arg_type_sec () const { return arg_type_second_; }

		int get_arg_val_fst ()  const { return arg_val_first_;  }
		int get_arg_val_sec ()  const { return arg_val_second_; }

		//! In - Out
		void print         (ostream& stream) const;

		//! Help
		bool isJumper ();
};

//! Operators
ostream& operator << (ostream& stream, instr_t obj);