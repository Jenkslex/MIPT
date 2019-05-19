//!{=================================================================================
//! @file    Syntax_analyzer.h
//! @date    2015-03-03 11:10
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement of syntax analyzer.
//!}=================================================================================
#pragma once


//!----------------------------------------------------------------------------------
//! Includings.
//!----------------------------------------------------------------------------------
#include "Data.h"
#include "BNode.h"
#include "Func_table.h"


//!----------------------------------------------------------------------------------
//! Syntax_Analyzer class.
//!----------------------------------------------------------------------------------
class Syntax_Analyzer
{
		int Lex_pointer_;
		Func_table_t** Func_tables_;

	public:
		//! Primary settings
		Syntax_Analyzer ();
		~Syntax_Analyzer ();

		// Analyzer
		BNode_t* Syntactic_Analysis (Data_t* Data[]);

	private:
		// Result
		void PrintResult (BNode_t* root);

		//! Function prototypes
		void           PrintProtos     ();
		void           FunctionsProto  (Data_t* Data[]);
		Func_table_t*  GetProto        (Data_t* Data[]);
		void           DeleteProtos    ();

		//! GetBody
		BNode_t*  GetBody      (Data_t* Data[], BNode_t* (Syntax_Analyzer::*GetTree) (Data_t* Data[]),
		                        instr_t Start_instr, instr_t Final_instr); 

		//! Functions
		BNode_t*  GetFunction  (Data_t* Data[]);
		BNode_t*  GetArg       (Data_t* Data[]);

		//! Syntactic Grammar
		BNode_t*  GetOperator  (Data_t* Data[]);
		BNode_t*  GetVarOp     (Data_t* Data[]); // <-
		BNode_t*  GetNew       (Data_t* Data[]); // born
		BNode_t*  GetNewArr    (Data_t* Data[]); // born A[]
		BNode_t*  GetNewVar    (Data_t* Data[]); // born a
		BNode_t*  GetCondOp    (Data_t* Data[]); // if while
		BNode_t*  GetReturn    (Data_t* Data[]); // return
		BNode_t*  GetInOut     (Data_t* Data[]); // In - Out

		//! Logic
		BNode_t*  GetCondition (Data_t* Data[]);
		BNode_t*  GetDisjunct  (Data_t* Data[]); // ||
		BNode_t*  GetConjunct  (Data_t* Data[]); // &&
		BNode_t*  GetNegate    (Data_t* Data[]); // !

		//! Expressions
		BNode_t*  GetE         (Data_t* Data[]); // + -
		BNode_t*  GetT         (Data_t* Data[]); // * /
		BNode_t*  GetP         (Data_t* Data[]); // ()
		BNode_t*  GetA         (Data_t* Data[]); // a[]
		BNode_t*  GetV         (Data_t* Data[]); // a, b, c
		BNode_t*  GetN         (Data_t* Data[]); // 123
		BNode_t*  GetF         (Data_t* Data[]); // function calls
		BNode_t* GetParams     (Data_t* Data[]);

		//! Diagnostics
		int GrammarError      (Data_t* Data, char* description);
		int Check             (instr_t instr, Data_t* Data[]);
};

