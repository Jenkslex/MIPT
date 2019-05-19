#include "stdafx.h"
#include "BNode.h"

//!----------------------------------------------------------------------------------
//! Definitions.
//!----------------------------------------------------------------------------------
#define TABS for (int tempcounter = 0; tempcounter < deep; tempcounter++)  \
	cout << "           ";


//! Primary settings
//! @{
//!==================================================================================
BNode_t::BNode_t (Data_t* Data):
	Data_  (Data   ),
	left_  (nullptr),
	right_ (nullptr)
{}
//!----------------------------------------------------------------------------------
BNode_t::BNode_t (Data_t* Data, BNode_t* left, BNode_t* right):
	Data_  (Data   ),
	left_  (left   ),
	right_ (right  )
{}
//!----------------------------------------------------------------------------------
BNode_t::~BNode_t () {}
//!==================================================================================
//! @}

//! Working with Node
//! @{
//!==================================================================================
BNode_t* BNode_t::copy ()
{
	BNode_t* copy_left_node  = nullptr;
	BNode_t* copy_right_node = nullptr;

	if (left_ ) copy_left_node  = left_ ->copy ();                     
	if (right_) copy_right_node = right_->copy ();                    

	Data_t*  copy_Data = Data_->copy ();                                      
	BNode_t* copy_node = new BNode_t (copy_Data, copy_left_node, copy_right_node);       

	return copy_node;
}
//!----------------------------------------------------------------------------------
void BNode_t::set_left  (BNode_t* left ) { left_  = left ; }
//!----------------------------------------------------------------------------------
void BNode_t::set_right (BNode_t* right) { right_ = right; }
//!----------------------------------------------------------------------------------
BNode_t* BNode_t::get_left  () const { return left_;  }
//!----------------------------------------------------------------------------------
BNode_t* BNode_t::get_right () const { return right_; }
//!----------------------------------------------------------------------------------
Data_t*  BNode_t::get_Data  () const { return Data_;  }
//!==================================================================================
//! @}


//! Help
//! @{
//!==================================================================================
int BNode_t::get_param_num () const
{
	int number = 0;

	if (left_ ) number += left_  -> get_param_num ();
	if (right_) number += right_ -> get_param_num ();

	if (Data_ -> get_type () == PARAM_VALUE) number++;

	return number;
}
//!==================================================================================
//! @}


//! Diagnostics
//! @{
//!==================================================================================
bool BNode_t::OK   ()
{
	if (!Data_) return false;

	return true;
}
//!----------------------------------------------------------------------------------
void BNode_t::Dump (int deep)
{
	if (left_ ) left_->Dump (deep + 1);

	TABS cout << "------node--Dump------" << endl;

	TABS cout << "node [0x" << this  << "]" << endl;
	TABS cout << "Data [0x" << Data_ << "]" << endl;
	if (Data_) { TABS cout << *Data_ << endl; }

	TABS cout << "left  = 0x" << left_  << endl;
	TABS cout << "right = 0x" << right_ << endl;

	TABS cout << "----------------------" << endl;

	if (right_) right_->Dump (deep + 1);

}
//!==================================================================================
//! @}


//! In-Out
//! @{
//!==================================================================================
void BNode_t::Export    (ostream& stream)
{
	stream << "node_start\n";
	stream << *Data_ << endl;

	if (left_ ) left_  -> Export (stream);
	if (right_) right_ -> Export (stream);

	stream << "node_end\n";
}
//!----------------------------------------------------------------------------------
void BNode_t::Visualise ()
{
	ofstream stream;
    stream.open("tree.gtr");

	stream << "~GTR 1.1" << endl;
	Export (stream);

	stream.close ();

	system ("gtr2png.exe");
}
//!==================================================================================
//! @}


//! Operators
//! @{
//!==================================================================================
#define BNODE_OPERATOR(oper, instr)                                               \
BNode_t* operator oper  (BNode_t& Left_Node, BNode_t& Right_Node)                 \
{                                                                                 \
	assert (&Left_Node );                                                         \
	assert (&Right_Node);                                                         \
	                                                                              \
	Data_t*  Oper_Data = new Data_t (INSTRUCTION, instr);                         \
	BNode_t* Oper_Node = new BNode_t (Oper_Data, &Left_Node, &Right_Node);        \
	                                                                              \
	return Oper_Node;                                                             \
}

BNODE_OPERATOR (+ , ADD);
BNODE_OPERATOR (- , SUB);
BNODE_OPERATOR (* , MUL);
BNODE_OPERATOR (/ , DIV);
BNODE_OPERATOR (&&, AND);
BNODE_OPERATOR (||, OR );

#undef BNODE_OPERATOR
//!==================================================================================
//! @}