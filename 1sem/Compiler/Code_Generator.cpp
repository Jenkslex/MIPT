#include "stdafx.h"
#include "Code_Generator.h"


//! Primary settings
//! @{
//!==================================================================================
Code_Generator::Code_Generator ():
	if_cnt_    (0),
	while_cnt_ (0),
	args_cnt_  (0)
{}
//!----------------------------------------------------------------------------------
Code_Generator::~Code_Generator ()
{}
//!==================================================================================
//! @}


//! Compile
//!==================================================================================
void Code_Generator::Generate      (BNode_t* root, char* FileName)
{
	assert (root    );
	assert (FileName);

	ofstream stream;
	stream.open(FileName);

	stream << "\tCall main" << endl;
	stream << "\tRet"       << endl;
	stream << endl;

	CodeGenBody (root, stream, RIGHT_DIR);

	stream << "\texit" << endl;

	stream.close ();
}
//!==================================================================================


//! Main selector
//!==================================================================================
void Code_Generator::CodeGenBody   (BNode_t* Node, ostream& stream, int direction)
{
	assert (Node);

	switch (Node -> get_Data () -> get_type ())
	{
		case BLANK       :
			if (direction == RIGHT_DIR) CodeGenBody (Node -> get_left  (), stream, direction);
			                            CodeGenBody (Node -> get_right (), stream, direction);
			if (direction == LEFT_DIR ) CodeGenBody (Node -> get_left  (), stream, direction);
			break;

		case NEW_FUNCTION: CodeGenFunc   (Node , stream); break;
		case PARAMETER   : ArgsHandling  (Node);          break;
		case PARAM_VALUE : CodeGenParam  (Node , stream); break;

		case NEW_VARIABLE: CodeGenNewVar (Node , stream); break;

		case INSTRUCTION :
			switch (Node -> get_Data () -> get_instr ())
			{
				case ASSIGN: CodeGenAssign (Node, stream); break;
				case RETURN: CodeGenReturn (Node, stream); break;
				case CALL  : CodeGenCall   (Node, stream); break;
				case IF    :
				case WHILE : CodeGenCondOp (Node, stream); break;
				case IN    : 
				case OUT   : CodeGenInOut  (Node, stream); break;

				default:
					assert (0);
			}
			break;

		default:
			assert (0);
	}
}
//!==================================================================================


//! Functions
//! @{
//!==================================================================================
void Code_Generator::CodeGenFunc   (BNode_t* Node, ostream& stream)
{
	stream << Node -> get_Data () -> get_string () << ":" << endl;
	
	stream << "\tCrtFrame" << endl << endl;

	if (Node -> get_left  ()) CodeGenBody  (Node -> get_left  (), stream, RIGHT_DIR);
	if (Node -> get_right ()) CodeGenBody  (Node -> get_right (), stream, RIGHT_DIR);

	stream << endl;

	args_cnt_ = 0;
	Vars_.clear ();
}
//!----------------------------------------------------------------------------------
void Code_Generator::ArgsHandling  (BNode_t* Node)
{
	BNode_t* Arg_Node = Node -> get_left ();                                          assert (Arg_Node);

	Vars_.push_back (Arg_Node -> get_Data () -> get_string ());

	args_cnt_++;
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenShift  (ostream& stream, string Var_Name)
{
	int vector_size = Vars_.size();
	int shift = -1;

	for (int i = 0; i < vector_size; i++)
		if (Vars_[i] == Var_Name)
			{ shift = i; break; }

	assert (shift != -1);

	shift = shift  + 1 - args_cnt_;

	if (shift <= 0) shift -= 2;

	stream << shift;
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenParam  (BNode_t* Node, ostream& stream)
{
	BNode_t* Param_Node = Node -> get_left ();

	CodeGenExp (Param_Node, stream);
}
//!==================================================================================
//! @}


//! Operators
//! @{
//!==================================================================================
void Code_Generator::CodeGenNewVar (BNode_t* Node, ostream& stream)
{
	Data_t* Var_Data = Node     -> get_Data   ();
	string  Var_Name = Var_Data -> get_string ();
	Vars_.push_back (Var_Name);
	
	stream << "\tNew" << endl;

	if (Node -> get_right ())
	{
		CodeGenExp (Node -> get_right (), stream);

		stream << "\tLoadLoc ";
		CodeGenShift (stream, Var_Name);
		stream << endl << endl;
	}
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenAssign (BNode_t* Node, ostream& stream)
{
	Data_t* Var_Data = Node -> get_left () -> get_Data   ();
	string  Var_Name = Var_Data -> get_string ();

	CodeGenExp (Node -> get_right (), stream);

	stream << "\tLoadLoc ";
	CodeGenShift (stream, Var_Name);
	stream << endl;
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenReturn (BNode_t* Node, ostream& stream)
{
	CodeGenExp (Node -> get_left (), stream);

	stream << "\tLoadReg ax" << endl;
	stream << "\tDltFrame"   << endl;
	stream << "\tRet"        << endl;
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenCall   (BNode_t* Node, ostream& stream)
{
	BNode_t* Arg_Node = Node -> get_right ();

	if (Arg_Node) CodeGenBody  (Arg_Node, stream, LEFT_DIR);
	int arg_num =  0;
	if (Arg_Node) arg_num = Arg_Node -> get_param_num ();

	BNode_t* Name_Node = Node      -> get_left   ();
	Data_t*  Name_Data = Name_Node -> get_Data   ();
	string   Func_Name = Name_Data -> get_string ();

	cout << "Arg_num: " << arg_num << endl;

	stream << "\tCall "  << Func_Name << endl;
	while (arg_num-- != 0) stream << "\tLoadReg dx" << endl;
	stream << endl;
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenInOut  (BNode_t* Node, ostream& stream)
{
	Data_t* Var_Data = Node -> get_left () -> get_Data   ();
	string  Var_Name = Var_Data -> get_string ();

	Data_t* InOut_Data = Node -> get_Data ();
	instr_t Instr_Data = InOut_Data -> get_instr ();

	switch (Instr_Data)
	{
		case IN : stream << "\tInLoc " ; break;
		case OUT: stream << "\tOutLoc "; break;
		
		default:
			assert (0);
	}

	CodeGenShift (stream, Var_Name);
	stream << endl << endl;
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenExp    (BNode_t* Node, ostream& stream)
{
	assert (Node);

	Data_t* Oper_Data = Node -> get_Data ();

	switch (Oper_Data -> get_type ())
	{
		case NUMBER     :
			stream << "\tPushConst " << Oper_Data -> get_number () << endl;
			break;

		case VARIABLE   :
			stream << "\tPushLoc ";
			CodeGenShift (stream, Oper_Data -> get_string ());
			stream << endl;
			break;

		case INSTRUCTION:
			switch (Oper_Data -> get_instr ())
			{
				case CALL:
					CodeGenCall (Node, stream);
					stream << "\tPushReg ax" << endl;
					break;

				case ADD:	case SUB:
				case MUL:	case DIV:
					CodeGenArithm (Node, stream);
					break;

				default:
					assert (0);
			}
			break;

		default:
			assert (0);
	}
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenArithm (BNode_t* Node, ostream& stream)
{
	Data_t* Oper_Data = Node -> get_Data ();

	CodeGenExp (Node -> get_left  (), stream);
	CodeGenExp (Node -> get_right (), stream);

	switch (Oper_Data -> get_instr ())
	{
		case ADD: stream << "\tAdd" << endl; break;
		case SUB: stream << "\tSub" << endl; break; 
		case MUL: stream << "\tMul" << endl; break;
		case DIV: stream << "\tDiv" << endl; break;

		default:  
			assert (0);
	}
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenCondOp (BNode_t* Node, ostream& stream)
{
	bool iswhile = (Node -> get_Data () -> get_instr () == WHILE);

	if ( iswhile) stream <<  "condwhile" <<  while_cnt_ << ":" << endl;

	CodeGenCond (Node -> get_left (), stream);

	stream << "\tJmpFalse ";
	if ( iswhile) stream << "endwhile" << while_cnt_ << endl;
	if (!iswhile) stream << "endif"    << if_cnt_    << endl;

	CodeGenBody (Node -> get_right (), stream, RIGHT_DIR);

	if ( iswhile) stream << "\tJmp condwhile" << while_cnt_ << endl;

	if ( iswhile) stream << "endwhile" << while_cnt_ << ":" << endl;
	if (!iswhile) stream << "endif"    << if_cnt_    << ":" << endl;

	stream << endl;

	if ( iswhile) while_cnt_++;
	if (!iswhile) if_cnt_++;
}
//!----------------------------------------------------------------------------------
void Code_Generator::CodeGenCond   (BNode_t* Node, ostream& stream)
{
	Data_t* Oper_Data  = Node      -> get_Data  ();
	instr_t Oper_instr = Oper_Data -> get_instr ();

	if ((Oper_instr == OR    ) ||
		(Oper_instr == AND   ) ||
		(Oper_instr == NEGATE))
	{
		CodeGenCond (Node -> get_left (), stream);
		if (Oper_instr != NEGATE)
			CodeGenCond (Node -> get_right (), stream);

		switch (Oper_instr)
		{
			case OR    : stream << "\tOr"     << endl; break;
			case AND   : stream << "\tAnd"    << endl; break;
			case NEGATE: stream << "\tNegate" << endl; break;

			default:  
				assert (0);
		}
	}
	else
	{
		CodeGenExp (Node -> get_left  (), stream);
		CodeGenExp (Node -> get_right (), stream);

		switch (Oper_instr)
		{
			case EQUAL : stream << "\tEqual" << endl; break;
			case NOTEQ : stream << "\tNoteq" << endl; break;
			case BOREQ : stream << "\tBoreq" << endl; break;
			case AOREQ : stream << "\tAoreq" << endl; break;
			case ABOVE : stream << "\tAbove" << endl; break;
			case BELOW : stream << "\tBelow" << endl; break;

			default:  
				assert (0);
		}
	}
}
//!==================================================================================
//! @}