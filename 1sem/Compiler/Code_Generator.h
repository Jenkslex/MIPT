//!{=================================================================================
//! @file    Code_Generator.h
//! @date    2015-30-03 10:06
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement Data for compile.
//!}=================================================================================
#pragma once


//!----------------------------------------------------------------------------------
//! Includings.
//!----------------------------------------------------------------------------------
#include "Data.h"
#include "BNode.h"
#include "Enums.h"


//!----------------------------------------------------------------------------------
//! Constants.
//!----------------------------------------------------------------------------------
const int LEFT_DIR  = 0;
const int RIGHT_DIR = 1;


//!----------------------------------------------------------------------------------
//! Code_Generator class.
//!----------------------------------------------------------------------------------
class Code_Generator
{
		int if_cnt_   ;
		int while_cnt_;

		int args_cnt_;
		vector <string> Vars_;

	public:
		//! Primary settings
		Code_Generator ();
		~Code_Generator ();

		//! Compile
		void Generate      (BNode_t* root, char* FileName);

	private:
		//! Main selector
		void CodeGenBody   (BNode_t* Node, ostream& stream, int direction);

		//! Functions
		void CodeGenFunc   (BNode_t* Node, ostream& stream);
		void ArgsHandling  (BNode_t* Node);
		void CodeGenShift  (ostream& stream, string Var_Name);
		void CodeGenParam  (BNode_t* Node, ostream& stream);

		void CodeGenNewVar (BNode_t* Node, ostream& stream);

		void CodeGenAssign (BNode_t* Node, ostream& stream);
		void CodeGenReturn (BNode_t* Node, ostream& stream);
		void CodeGenCall   (BNode_t* Node, ostream& stream);
		void CodeGenInOut  (BNode_t* Node, ostream& stream);

		void CodeGenExp    (BNode_t* Node, ostream& stream);
		void CodeGenArithm (BNode_t* Node, ostream& stream);
		void CodeGenCondOp (BNode_t* Node, ostream& stream);
		void CodeGenCond   (BNode_t* Node, ostream& stream);
};

