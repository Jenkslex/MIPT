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
#include "Data.h"
#include "Enums.h"


//!----------------------------------------------------------------------------------
//! BTree class.
//!----------------------------------------------------------------------------------
class BNode_t
{
		Data_t*  Data_ ;

		BNode_t* left_ ;
		BNode_t* right_;

	public:
		//! Primary settings
		BNode_t  (Data_t* Data);
		BNode_t  (Data_t* Data, BNode_t* left, BNode_t* right);
		~BNode_t ();


		//! Working with Node
		BNode_t* copy  ();

		void set_left  (BNode_t* left );
		void set_right (BNode_t* right);

		BNode_t* get_left  () const;
		BNode_t* get_right () const;
		Data_t*  get_Data  () const;


		//! Help
		int get_param_num () const;

		//! Diagnostics
		bool OK     ();
		void Dump   (int deep = 0);
		

		//! In-Out
		void Export    (ostream& stream);
		void Visualise ();
};


//! Operators
BNode_t* operator +  (BNode_t& Left_Node, BNode_t& Right_Node);
BNode_t* operator -  (BNode_t& Left_Node, BNode_t& Right_Node);
BNode_t* operator *  (BNode_t& Left_Node, BNode_t& Right_Node);
BNode_t* operator /  (BNode_t& Left_Node, BNode_t& Right_Node);
BNode_t* operator || (BNode_t& Left_Node, BNode_t& Right_Node);
BNode_t* operator && (BNode_t& Left_Node, BNode_t& Right_Node);
