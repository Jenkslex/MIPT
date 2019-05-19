#include "stdafx.h"
#include "Syntax_Analyzer.h"


//! Primary settings
//! @{
//!==================================================================================
Syntax_Analyzer::Syntax_Analyzer ():
	Lex_pointer_ (0),
	Func_tables_ ()
{}
//!----------------------------------------------------------------------------------
Syntax_Analyzer::~Syntax_Analyzer ()
{}
//!==================================================================================
//! @}


//! Analyzer
//!==================================================================================
BNode_t*  Syntax_Analyzer::Syntactic_Analysis (Data_t* Data[])
{
	assert (Data);

	Lex_pointer_ = 0;
	FunctionsProto (Data);

	PrintProtos ();

	BNode_t* root = GetBody (Data, &Syntax_Analyzer::GetFunction, NEW, EXIT);

	DeleteProtos ();

	PrintResult (root);

	return root;
}
//!----------------------------------------------------------------------------------
void      Syntax_Analyzer::PrintResult        (BNode_t* root)
{
	cout << "Syntax analyzer:" << endl;
	root -> Dump ();
}
//!==================================================================================


//! Function prototypes
//! @{
//!==================================================================================
void           Syntax_Analyzer::PrintProtos     ()
{
	cout << "Function prototypes:\n";

	int i = 0;
	while (Func_tables_[i]) cout << *(Func_tables_[i++]) << endl; 
}
//!----------------------------------------------------------------------------------
void           Syntax_Analyzer::FunctionsProto  (Data_t* Data[])
{
	Func_tables_ = new Func_table_t* [MAX_FUNC_NUM];
	memset (Func_tables_, 0, sizeof (Func_table_t*) * MAX_FUNC_NUM);

	int Func_pointer = 0;

	do Func_tables_[Func_pointer] = GetProto (Data);
	while (Func_tables_[Func_pointer++]);

	Lex_pointer_--;
}
//!----------------------------------------------------------------------------------
Func_table_t*  Syntax_Analyzer::GetProto        (Data_t* Data[])
{
	Check (NEW, Data);

	if (Data[Lex_pointer_] -> isinstrno (MAIN)) return nullptr;

	string        Func_Name = Data[Lex_pointer_++] -> get_string ();
	Func_table_t* Function  = new Func_table_t (Func_Name);

	Check (OPCBRC, Data);

	while (!(Data[Lex_pointer_] -> isinstrno (CLCBRC)))
	{
		Check (NEW, Data);

		Function -> Add_Arg (ARG_VALUE, Data[Lex_pointer_++] -> get_string ());

		if (Data[Lex_pointer_] -> isinstrno (CLCBRC)) break;

		Check (COMMA, Data);
	}

	Check (CLCBRC, Data);
	Check (ENDOP , Data);

	return Function;
}
//!----------------------------------------------------------------------------------
void           Syntax_Analyzer::DeleteProtos    ()
{
	int i = 0;
	while (Func_tables_[i]) delete Func_tables_[i++]; 

	delete [] Func_tables_;
}
//!==================================================================================
//! @}


//! GetBody
//!==================================================================================
BNode_t*       Syntax_Analyzer::GetBody         (Data_t* Data[], BNode_t* (Syntax_Analyzer::*GetTree) (Data_t* Data[]), 
                                                 instr_t Start_instr, instr_t Final_instr)
{
	Check (Start_instr, Data);

	if (Data[Lex_pointer_] -> isinstrno (Final_instr)) 
		{ Lex_pointer_++; return nullptr; }

	BNode_t* Oper_Node = (this ->* GetTree) (Data);                                         

	if (Data[Lex_pointer_] -> isinstrno (Final_instr)) 
		{ Lex_pointer_++; return Oper_Node; }

	Data_t*  Blank_Data = new Data_t  ();                                           
	BNode_t* root       = new BNode_t (Blank_Data, Oper_Node, nullptr);    

	BNode_t* Curr_Node  = root;
	BNode_t* Blank_Node = nullptr;

	while (!(Data[Lex_pointer_] -> isinstrno (Final_instr)))
	{
		Oper_Node = (this ->* GetTree) (Data);

		if (Data[Lex_pointer_] -> isinstrno (Final_instr)) 
			{ Curr_Node -> set_right (Oper_Node); break; }

		Blank_Data = new Data_t  ();
		Blank_Node = new BNode_t (Blank_Data, Oper_Node, nullptr);

		Curr_Node -> set_right (Blank_Node);
		Curr_Node = Blank_Node;
	}

	Check (Final_instr, Data);

	return root;
}
//!==================================================================================


//! Functions
//! @{
//!==================================================================================
BNode_t*       Syntax_Analyzer::GetFunction     (Data_t* Data[])
{
	Data_t* Func_Name = nullptr;

	if (Data[Lex_pointer_] -> isinstrno (MAIN)) 
		Func_Name = new Data_t (NEW_FUNCTION, "main");
	else
	{
		Check (NEW, Data);
		Func_Name = new Data_t (NEW_FUNCTION, Data[Lex_pointer_] -> get_string ());
	}

	assert (Func_Name);
	Lex_pointer_++;

	BNode_t* Func_Node = new BNode_t (Func_Name);

	BNode_t* Arg_Node  = GetBody (Data, &Syntax_Analyzer::GetArg     , OPCBRC, CLCBRC);
	BNode_t* Body_Node = GetBody (Data, &Syntax_Analyzer::GetOperator, OPBODY, CLBODY);

	Func_Node -> set_left  (Arg_Node );
	Func_Node -> set_right (Body_Node);

	return Func_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetArg          (Data_t* Data[])
{
	if (Data[Lex_pointer_] -> isinstrno (CLCBRC)) return nullptr;

	Check (NEW, Data);

	BNode_t*       Arg_Node = GetNewArr (Data);
	if (!Arg_Node) Arg_Node = GetNewVar (Data);

	assert (Arg_Node);

	Data_t*  Param_Data = new Data_t  (PARAMETER, "function");
	BNode_t* Param_Node = new BNode_t (Param_Data);

	Param_Node -> set_left (Arg_Node);

	if (Data[Lex_pointer_] -> isinstrno (COMMA)) Lex_pointer_++;

	return Param_Node;
}
//!==================================================================================
//! @}


//! Syntax Grammar
//! @{
//!==================================================================================
BNode_t*       Syntax_Analyzer::GetOperator     (Data_t* Data[])
{
	switch (Data[Lex_pointer_] -> get_type ())
	{
		case OPER_STRING:		 return GetVarOp  (Data);

		case INSTRUCTION:
				switch (Data[Lex_pointer_] -> get_instr ())
				{
					case    NEW: return GetNew    (Data);
					case     IF:
					case  WHILE: return GetCondOp (Data);
					case RETURN: return GetReturn (Data);
					case     IN:
					case    OUT: return GetInOut  (Data);

					default: assert (0);
				}

		default: assert (0);
	}

	return nullptr;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetVarOp        (Data_t* Data[])
{
	BNode_t*       Var_Node = GetF (Data);
	if (Var_Node)
	{
		Check (ENDOP, Data);
		return Var_Node;
	}

		           Var_Node = GetA (Data);
	if (!Var_Node) Var_Node = GetV (Data);
	if (!Var_Node) return nullptr;

	if ((Data[Lex_pointer_] -> get_type ()) != INSTRUCTION) return nullptr;

	Data_t*  Oper_Data  = Data[Lex_pointer_++] -> copy ();

	BNode_t* Exp_Node = GetE (Data);

	BNode_t* Oper_Node  = new BNode_t (Oper_Data, Var_Node, Exp_Node);

	Check (ENDOP, Data);

	return Oper_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetNew          (Data_t* Data[])
{
	Check (NEW, Data);

	BNode_t*       New_Node = GetNewArr (Data);
	if (!New_Node) New_Node = GetNewVar (Data);
	if (!New_Node) return nullptr;

	if (Data[Lex_pointer_] -> isinstrno (ASSIGN))
	{
		Lex_pointer_++;

		Data_t* New_Data  = New_Node -> get_Data ();
		lexem_t Data_type = New_Data -> get_type ();

		if (Data_type == NEW_VARIABLE)
		{
			BNode_t* Exp_Node = GetE (Data);
			New_Node -> set_right (Exp_Node);
		}
		if (Data_type == NEW_ARRAY)
		{

		}
	}

	Check (ENDOP, Data);

	return New_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetNewArr       (Data_t* Data[])
{
	if (  Data[Lex_pointer_] -> get_type () != OPER_STRING)  return nullptr;
	if (!(Data[Lex_pointer_] -> isinstrno (OPARR)))          return nullptr;

	string   New_Arr_Name = Data[Lex_pointer_++] -> get_string ();
	Data_t*  New_Arr_Data = new Data_t (NEW_ARRAY, New_Arr_Name);
																						
	Check (OPARR, Data);
	BNode_t* Num_Node = GetN (Data);
	Check (CLARR, Data);

	BNode_t* New_Arr_Node = new BNode_t (New_Arr_Data, Num_Node, nullptr);

	return New_Arr_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetNewVar       (Data_t* Data[])
{
	if (Data[Lex_pointer_] -> get_type () != OPER_STRING)  return nullptr;

	string   New_Var_Name = Data[Lex_pointer_++] -> get_string ();
	Data_t*  New_Var_Data = new Data_t (NEW_VARIABLE, New_Var_Name);                 
	BNode_t* New_Var_Node = new BNode_t (New_Var_Data);              

	return New_Var_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetCondOp       (Data_t* Data[])
{
	Data_t*  Oper_Data  = Data[Lex_pointer_++] -> copy ();
	BNode_t* Oper_Node  = new BNode_t (Oper_Data);                   

	BNode_t* Cond_Node  = GetCondition (Data);
	BNode_t* Body_Node  = GetBody      (Data, &Syntax_Analyzer::GetOperator, OPBODY, CLBODY);

	Oper_Node-> set_left  (Cond_Node);
	Oper_Node-> set_right (Body_Node);
	 
	return Oper_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetReturn       (Data_t* Data[])
{
	Check (RETURN, Data);

	BNode_t* Val_Node = GetE (Data);

	Data_t*  Ret_Data = new Data_t (INSTRUCTION, RETURN);
	BNode_t* Ret_Node = new BNode_t (Ret_Data, Val_Node, nullptr);

	Check (ENDOP, Data);

	return Ret_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetInOut        (Data_t* Data[])
{
	Data_t* InOut_Data = Data[Lex_pointer_++] -> copy ();

	BNode_t* Val_Node = GetV (Data);
	BNode_t* Ret_Node = new BNode_t (InOut_Data, Val_Node, nullptr);

	Check (ENDOP, Data);

	return Ret_Node;
}
//!==================================================================================
//! @}


//! Logic
//! @{
//!==================================================================================
BNode_t*       Syntax_Analyzer::GetCondition    (Data_t* Data[])
{
	Check (OPCBRC, Data);

	BNode_t* Cond_Node = nullptr;

	if ((Data[Lex_pointer_] -> isinstrno (OPCBRC)) ||
		(Data[Lex_pointer_] -> isinstrno (NEGATE)))
		  Cond_Node = GetDisjunct (Data);
	else
	{
		BNode_t* First_Elem  = GetE (Data); 
		Data_t* Cond_Data    =  Data[Lex_pointer_++] -> copy ();
		BNode_t* Second_Elem = GetE (Data);

		Cond_Node         = new BNode_t (Cond_Data, First_Elem, Second_Elem);
	}

	Check (CLCBRC, Data);

	return Cond_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetDisjunct     (Data_t* Data[])
{
	BNode_t* Con_Node = GetConjunct (Data);

	while (Data[Lex_pointer_] -> isinstrno (OR))
	{
		Lex_pointer_++;
		BNode_t* Con_Node_temp = GetConjunct (Data);

		Con_Node = (*Con_Node) || (*Con_Node_temp);
	}

	return Con_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetConjunct     (Data_t* Data[])
{
	BNode_t* Neg_Node = GetNegate (Data);

	while (Data[Lex_pointer_] -> isinstrno (AND))
	{                         
		Lex_pointer_++;
		BNode_t* Neg_Node_temp = GetNegate (Data);

		Neg_Node = (*Neg_Node) && (*Neg_Node_temp);
	}

	return Neg_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetNegate       (Data_t* Data[])
{
	if (Data[Lex_pointer_] -> isinstrno (NEGATE))
	{
		Data_t* Data_Neg = Data[Lex_pointer_++] -> copy ();

		BNode_t* Cond_Node = GetCondition (Data);
		BNode_t* Neg_Node  = new BNode_t (Data_Neg, Cond_Node, NULL);

		return Neg_Node;
	}
	else return GetCondition (Data);

	return nullptr;
}
//!==================================================================================
//! @}


//! Expressions
//! @{
//!==================================================================================
BNode_t*       Syntax_Analyzer::GetE            (Data_t* Data[])
{
	BNode_t* Val_Node = GetT (Data); 

	while ((Data[Lex_pointer_] -> isinstrno (ADD)) ||
	       (Data[Lex_pointer_] -> isinstrno (SUB)))
	{
		instr_t Operator =  Data[Lex_pointer_++] -> get_instr ();

		BNode_t* Val_Node_temp = GetT (Data);

		if (Operator == ADD) Val_Node = (*Val_Node) + (*Val_Node_temp);
		if (Operator == SUB) Val_Node = (*Val_Node) - (*Val_Node_temp);
	}
	return Val_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetT            (Data_t* Data[])
{
	BNode_t* Val_Node = GetP (Data);

	while ((Data[Lex_pointer_] -> isinstrno (MUL)) ||
	       (Data[Lex_pointer_] -> isinstrno (DIV)))
	{
		instr_t Operator =  Data[Lex_pointer_++] -> get_instr ();

		BNode_t* Val_Node_temp = GetP (Data);

		if (Operator == MUL) Val_Node = (*Val_Node) * (*Val_Node_temp);
		if (Operator == DIV) Val_Node = (*Val_Node) / (*Val_Node_temp);
	}

	return Val_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetP            (Data_t* Data[])
{
	if (Data[Lex_pointer_] -> isinstrno (OPCBRC))
	{
		Lex_pointer_++;
		BNode_t* Bracket_Node = GetE (Data);

		Check (CLCBRC, Data);

		return Bracket_Node;
	}
	else
	{
		BNode_t* Val_Node = nullptr;

		Val_Node = GetF (Data); if (Val_Node) return Val_Node;
		Val_Node = GetA (Data); if (Val_Node) return Val_Node;
		Val_Node = GetV (Data); if (Val_Node) return Val_Node;
		Val_Node = GetN (Data); if (Val_Node) return Val_Node;

		GrammarError (Data[Lex_pointer_], "Wrong P");

		return nullptr;
	}
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetA            (Data_t* Data[])
{
	if (  Data[Lex_pointer_] -> get_type () != OPER_STRING) return nullptr;
	if (!(Data[Lex_pointer_] -> isinstrno         (OPARR))) return nullptr;

	string   Arr_Name = Data[Lex_pointer_++] -> get_string ();
	Data_t*  Arr_Data = new Data_t (ARRAY, Arr_Name);

	Check (OPARR, Data);
	BNode_t* Num_Node = GetE (Data);
	Check (CLARR, Data);

	BNode_t* Arr_Node = new BNode_t (Arr_Data, Num_Node, nullptr);

	return Arr_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetV            (Data_t* Data[])
{
	if (Data[Lex_pointer_] -> get_type () != OPER_STRING) return nullptr;

	string   Var_Name = Data[Lex_pointer_++] -> get_string();
	Data_t*  Var_Data = new Data_t (VARIABLE, Var_Name);
	BNode_t* Var_Node = new BNode_t (Var_Data);

	return Var_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetN            (Data_t* Data[])
{
	if (Data[Lex_pointer_] -> get_type () != NUMBER) return nullptr;

	Data_t*  Val_Data = Data[Lex_pointer_++] -> copy ();
	BNode_t* Val_Node = new BNode_t (Val_Data);

	return Val_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetF            (Data_t* Data[])
{
	if (Data[Lex_pointer_] -> get_type () != OPER_STRING) return nullptr;

	string Func_Name = Data[Lex_pointer_] -> get_string ();

	int i = 0;
	while (Func_tables_[i])
	{ if (Func_Name == Func_tables_[i] -> get_name ()) break; i++; }

	if (!Func_tables_[i]) return nullptr;

	Lex_pointer_++;

	Data_t*  Func_Data = new Data_t  (FUNCTION, Func_Name);
	BNode_t* Func_Node = new BNode_t (Func_Data);

	BNode_t* Arg_Node  = GetBody (Data, &Syntax_Analyzer::GetParams, OPCBRC, CLCBRC);

	Data_t*  Call_Data = new Data_t  (INSTRUCTION, CALL);
	BNode_t* Call_Node = new BNode_t (Call_Data, Func_Node, Arg_Node);

	return Call_Node;
}
//!----------------------------------------------------------------------------------
BNode_t*       Syntax_Analyzer::GetParams       (Data_t* Data[])
{
	if (Data[Lex_pointer_] -> isinstrno (CLCBRC)) return nullptr;

	BNode_t* Arg_Node   = GetE (Data);
	Data_t*  Param_Data = new Data_t  (PARAM_VALUE);
	BNode_t* Param_Node = new BNode_t (Param_Data, Arg_Node, nullptr);

	if (Data[Lex_pointer_] -> isinstrno (COMMA)) Lex_pointer_++;

	return Param_Node;
}
//!==================================================================================
//! @}


//! Working with errors
//! @{
//!==================================================================================
int            Syntax_Analyzer::GrammarError    (Data_t* Data, char* description)
{
	cout << "Grammar Error" << endl;
	cout << "Wrong lexem "  << endl;
	cout << "On lexem "     << Lex_pointer_ << endl;
	cout << *Data << endl;
	cout << description << endl;

	_getch ();
	exit (1);

	return 1;
}
//!----------------------------------------------------------------------------------
int            Syntax_Analyzer::Check           (instr_t instr, Data_t* Data[])
{
	if (!(Data[Lex_pointer_] -> isinstrno (instr)))
	{
		cout << "Grammar Error"     << endl;
		cout << "Wrong lexem "      << endl;
		cout << *Data[Lex_pointer_] << endl;
		cout << "Need instruction " << instr << endl;

		_getch ();
		exit (1);
	}
	Lex_pointer_++;

	return 0;
}
//!==================================================================================
//! @}