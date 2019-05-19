//!{=================================================================================
//! @file    Tree.h
//! @date    2014-14-11 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement tree.
//!}=================================================================================


//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include "stdafx.h"


//!----------------------------------------------------------------------------------
//! Definitions.
//!----------------------------------------------------------------------------------
#define ISZERO( NUMBER )          (((NUMBER) < 0.0000001) && ((NUMBER) > -0.0000001))
#define ISNUMBER( Data, number)                      \
			((typeof_Data (Data) == NUMBER) && (ISZERO (Data_number (Data) - (number))))


//!----------------------------------------------------------------------------------
//! Constants.
//!----------------------------------------------------------------------------------
const int MAX_STRING_SIZE = 120;
const int MAX_VAR_LEN     = 10;


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Diff
node_t* Diff    (node_t* node, char variable[]);

//! Cases
node_t* DiffVar    (node_t* node, char variable[]);
node_t* DiffNum    (node_t* node);
node_t* DiffArithm (node_t* node, char variable[]);
node_t* DiffMul    (node_t* node, char variable[]);
node_t* DiffDiv    (node_t* node, char variable[]);

//! Functions
node_t* Diff_deg   (node_t* node, char variable[]);
node_t* Diff_sin   (node_t* node, char variable[]);
node_t* Diff_cos   (node_t* node, char variable[]);
node_t* Diff_asin  (node_t* node, char variable[]);
node_t* Diff_acos  (node_t* node, char variable[]);
node_t* Diff_ln    (node_t* node, char variable[]);
node_t* Diff_log   (node_t* node, char variable[]);

//! Simplification
node_t* Simplification (node_t* node);

//! Ways of simplification
int const_folding  (node_t** node);
int remove_inanity (node_t** node);
int AddZero        (node_t** node);
int MulOnZero      (node_t** node);
int MulOnOne       (node_t** node);
int DegOne         (node_t** node);
int DegZero        (node_t** node);

 
//! Print_expression
int Print_exp    (node_t* node, int old_priority = 0);


//! Main
//!==================================================================================
int main()
{
	char String[MAX_STRING_SIZE] = {};

	gets (String);

	Data_t** Data = Lexical_Analysis   (String);                                     assert (Data);
	node_t*  tree = Syntactic_Analysis (Data);                                       assert (tree);

	node_Dump (tree);

	Print_exp (tree); printf ("\n");

	char variable[MAX_VAR_LEN] = {}; 
	printf ("Enter the diffed variable: ");
	scanf ("%s", variable);

	node_t* diffed_tree = Diff (tree, variable);                                     assert (diffed_tree);

	node_Dump (diffed_tree);

	Print_exp (tree);               printf ("\n");
	Print_exp (diffed_tree);        printf ("\n");

	node_t* simplificated_tree = Simplification (diffed_tree);                       assert (simplificated_tree); 

	node_Dump (simplificated_tree);

	Print_exp (tree              ); printf ("\n");
	Print_exp (diffed_tree       ); printf ("\n");
	Print_exp (simplificated_tree); printf ("\n");

	_getch();
	return 0;
}
//!==================================================================================


//! Diff
//!==================================================================================
node_t* Diff (node_t* node, char variable[])
{
	assert (node     != NULL);
	assert (variable != NULL);

	node_t* copy_node = node_copy (node);                                             assert (copy_node);

	if (typeof_Data (copy_node->Data) == VARIABLE   ) return DiffVar (copy_node, variable);
	if (typeof_Data (copy_node->Data) == NUMBER     ) return DiffNum (copy_node);

	if (typeof_Data (copy_node->Data) == INSTRUCTION)
	{
		switch (Data_instruction (copy_node->Data))
		{
			case 0:  return DiffArithm (copy_node, variable);
			case 1:  return DiffArithm (copy_node, variable);
			case 2:  return DiffMul    (copy_node, variable);
			case 3:  return DiffDiv    (copy_node, variable);

			case 6:  return Diff_deg   (copy_node, variable);
			case 60: return Diff_sin   (copy_node, variable);
			case 61: return Diff_cos   (copy_node, variable);

			default: break;
		}
	}

	return NULL;
}
//!==================================================================================


//! Cases
//! @{
//!==================================================================================
node_t* DiffVar    (node_t* node, char variable[])
{
	assert (node     != NULL);
	assert (variable != NULL);

	char VarName[MAX_VAR_LEN] = {};
	strcpy (VarName, Data_String (node->Data));

	Data_t* NewData = NULL;

	node_dtor (node);
	
	if (strcmp (VarName, variable) == 0) { NewData = new_Data (NUMBER, 1.0);          assert (NewData); }
	else                           		 { NewData = new_Data (NUMBER, 0.0);          assert (NewData); }

	node = new_node (NewData, NULL, NULL);                                            assert (node);

	return node;
}
//!----------------------------------------------------------------------------------
node_t* DiffNum    (node_t* node)
{
	assert (node);

	node_dtor (node);

	Data_t* NewData = new_Data (NUMBER, 0.0);                                         assert (NewData);
	node = new_node (NewData, NULL, NULL);                                            assert (new_node);

	return node;
}
//!----------------------------------------------------------------------------------
node_t* DiffArithm (node_t* node, char variable[])
{
	assert (node       );
	assert (variable   );
	assert (node->left );
	assert (node->right);

	node_t* diff_left  = Diff (node->left , variable);                                assert (diff_left );
	node_t* diff_right = Diff (node->right, variable);                                assert (diff_right);

	Data_t* copy_Data = Data_copy (node->Data);                                       assert (copy_Data);

	node_dtor (node);

	node = new_node (copy_Data, diff_left, diff_right);                               assert (node);

	return node;
}
//!----------------------------------------------------------------------------------
node_t* DiffMul    (node_t* node, char variable[])
{
	assert (node       );
	assert (node->left );
	assert (node->right);

	node_t* copy_left  = node_copy (node->left );                                     assert (copy_left );
	node_t* copy_right = node_copy (node->right);                                     assert (copy_right);

	node_t* diff_left  = Diff (node->left , variable);                                assert (diff_left );
	node_t* diff_right = Diff (node->right, variable);                                assert (diff_right);

	Data_t* AddData = new_Data (INSTRUCTION, 0);                                      assert (AddData);
	Data_t* MulData = new_Data (INSTRUCTION, 2);                                      assert (MulData);

	node_dtor (node);

	node_t* first_subtree  = new_node (MulData, diff_left, copy_right);               assert (first_subtree );
	node_t* second_subtree = new_node (MulData, copy_left, diff_right);               assert (second_subtree);

	node = new_node (AddData, first_subtree , second_subtree);                        assert (node);

	return node;
}
//!----------------------------------------------------------------------------------
node_t* DiffDiv    (node_t* node, char variable[])
{
	assert (node       );
	assert (variable   );
	assert (node->left );
	assert (node->right);

	node_t* copy_left  = node_copy (node->left);                                      assert (copy_left );
	node_t* copy_right = node_copy (node->right);                                     assert (copy_right);

	node_t* diff_left  = Diff (node->left , variable);                                assert (diff_left );
	node_t* diff_right = Diff (node->right, variable);                                assert (diff_right);

	Data_t* SubData = new_Data (INSTRUCTION, 1);                                      assert (SubData);
	Data_t* MulData = new_Data (INSTRUCTION, 2);                                      assert (MulData);
	Data_t* DivData = new_Data (INSTRUCTION, 3);                                      assert (DivData);

	node_dtor (node);

	node_t* first_smalltree  = new_node (MulData, diff_left, copy_right);             assert (first_smalltree );
	node_t* second_smalltree = new_node (MulData, copy_left, diff_right);             assert (second_smalltree);

	node_t* first_subtree    = new_node (SubData, first_smalltree, second_smalltree); assert (first_subtree );
	node_t* second_subtree   = new_node (MulData, copy_right, copy_right);            assert (second_subtree);

	node = new_node (DivData, first_subtree , second_subtree);                        assert (node);

	return node;
}
//!==================================================================================
//! @}

//! Functions
//! @{
//!==================================================================================
node_t* Diff_deg   (node_t* node, char variable[])
{
	assert (node       );
	assert (variable   );
	assert (node->left );
	assert (node->right);

	node_t* copy_Exp = node_copy (node->left );                                       assert (copy_Exp);
	node_t* copy_deg = node_copy (node->right);                                       assert (copy_deg);
	node_t* diff_Exp = Diff (node->left, variable);                                   assert (diff_Exp);

	Data_t* NumData = new_Data (NUMBER,      1.0);                                    assert (NumData);
	Data_t* SubData = new_Data (INSTRUCTION, 1  );                                    assert (SubData);
	Data_t* MulData = new_Data (INSTRUCTION, 2  );                                    assert (MulData);
	Data_t* DegData = new_Data (INSTRUCTION, 6  );                                    assert (DegData);

	node_dtor (node);

	node_t* Num_node = new_node (NumData, NULL, NULL);                                assert (Num_node);
	node_t* new_deg  = new_node (SubData, copy_deg, Num_node);                        assert (new_deg);
	node_t* new_Exp  = new_node (DegData, copy_Exp, new_deg);                         assert (new_Exp);
	node_t* new_coef = new_node (MulData, copy_deg, diff_Exp);                        assert (new_coef);
	node             = new_node (MulData, new_coef, new_Exp);                         assert (node);

	return node;
}
//!----------------------------------------------------------------------------------
node_t* Diff_sin   (node_t* node, char variable[])
{
	assert (node      );
	assert (variable  );
	assert (node->left);

	node_t* copy_Arg = node_copy (node->left);                                        assert (copy_Arg);
	node_t* diff_Arg = Diff (node->left, variable);                                   assert (diff_Arg);

	Data_t* MulData = new_Data (INSTRUCTION, 2 );                                     assert (MulData); 
	Data_t* cosData = new_Data (INSTRUCTION, 61);                                     assert (cosData); 

	node_dtor (node);

	node_t* Func_Diff = new_node (cosData, copy_Arg, NULL);                           assert (Func_Diff);
	node              = new_node (MulData, diff_Arg, Func_Diff);                      assert (node     );

	return node;
}
//!----------------------------------------------------------------------------------
node_t* Diff_cos   (node_t* node, char variable[])
{
	assert (node      );
	assert (variable  );
	assert (node->left);

	node_t* copy_Arg = node_copy (node->left);                                        assert (copy_Arg);
	node_t* diff_Arg = Diff (node->left, variable);                                   assert (diff_Arg);

	Data_t* MulData   = new_Data (INSTRUCTION, 2 );                                   assert (MulData); 
	Data_t* sinData   = new_Data (INSTRUCTION, 60);                                   assert (sinData);
	Data_t* MinusData = new_Data (NUMBER, -1.0);                                      assert (MinusData);  

	node_dtor (node);

	node_t* Minus_node  = new_node (MinusData, NULL, NULL);                           assert (Minus_node);
	node_t* Func_Diff   = new_node (sinData, copy_Arg, NULL);
	node_t* Mul_node    = new_node (MulData, Minus_node, diff_Arg);                   assert (Func_Diff);
	node                = new_node (MulData, Mul_node, Func_Diff);                    assert (node     );

	return node;
}
//!----------------------------------------------------------------------------------
node_t* Diff_asin  (node_t* node, char variable[]);
//!----------------------------------------------------------------------------------
node_t* Diff_acos  (node_t* node, char variable[]);
//!----------------------------------------------------------------------------------
node_t* Diff_ln    (node_t* node, char variable[]);
//!----------------------------------------------------------------------------------
node_t* Diff_log   (node_t* node, char variable[]);
//!==================================================================================
//! @}

//! Simplification
//!==================================================================================
node_t* Simplification (node_t* node)
{
	assert (node);

	node_t* copy_node = node_copy (node);                                             assert (copy_node); 

	int was_modified = 0;

	do
	{
		was_modified = 0;

		was_modified += const_folding  (&copy_node);
		was_modified += remove_inanity (&copy_node);
	}
	while (was_modified != 0);

	return copy_node;
}
//!==================================================================================

//! Ways of simplification
//! @{
//!==================================================================================
int const_folding  (node_t** node)
{
	assert (node );
	assert (*node);

	int was_modified = 0;

	if (typeof_Data ((*node)->Data) == NUMBER  ) return 0;
	if (typeof_Data ((*node)->Data) == VARIABLE) return 0;

	assert ((*node)->left );

	if (isFunction (Data_instruction ((*node)->Data))) return const_folding (&((*node)->left ));

	assert ((*node)->right);

	was_modified += const_folding (&((*node)->left ));
	was_modified += const_folding (&((*node)->right));

	if ((typeof_Data ((*node)->left->Data ) == NUMBER) &&
		(typeof_Data ((*node)->right->Data) == NUMBER))
	{
		double first_number  = Data_number ((*node)->left->Data );
		double second_number = Data_number ((*node)->right->Data);

		double new_number = 0;

		if (Data_instruction ((*node)->Data) == 0) new_number = first_number + second_number;
		if (Data_instruction ((*node)->Data) == 1) new_number = first_number - second_number;
		if (Data_instruction ((*node)->Data) == 2) new_number = first_number * second_number;
		if (Data_instruction ((*node)->Data) == 3) new_number = first_number / second_number;

		node_dtor (*node);

		Data_t* NumData = new_Data (NUMBER, new_number);                              assert (NumData);   
		*node           = new_node (NumData, NULL, NULL);                             assert (node   );

		return 1;
	}

	return was_modified;
}
//!----------------------------------------------------------------------------------
int remove_inanity (node_t** node)
{
	assert (node );
	assert (*node);

	int was_modified = 0;

	if (typeof_Data ((*node)->Data) == NUMBER  ) return 0;
	if (typeof_Data ((*node)->Data) == VARIABLE) return 0;

	assert ((*node)->left );

	if (isFunction (Data_instruction ((*node)->Data))) return remove_inanity (&((*node)->left ));

	assert ((*node)->right);

	was_modified += remove_inanity (&((*node)->left ));
	was_modified += remove_inanity (&((*node)->right));

	if (INSTRUCTIONNO ((*node)->Data, 0)) return AddZero (node);
	if (INSTRUCTIONNO ((*node)->Data, 2)) if (MulOnZero (node) != 0) return 1;
	if (INSTRUCTIONNO ((*node)->Data, 2)) if (MulOnOne  (node) != 0) return 1;
	if (INSTRUCTIONNO ((*node)->Data, 6)) if (DegOne    (node) != 0) return 1;
	if (INSTRUCTIONNO ((*node)->Data, 6)) if (DegZero   (node) != 0) return 1;

	return was_modified;
}
//!----------------------------------------------------------------------------------
int AddZero        (node_t** node)
{
	assert (node);
	assert (*node);
	assert (typeof_Data ((*node)->Data) == INSTRUCTION);
	assert (Data_instruction ((*node)->Data) == 0);
	assert ((*node)->left );
	assert ((*node)->right);

	if ((ISNUMBER( (*node)->left ->Data, 0.0)) ||
		(ISNUMBER( (*node)->right->Data, 0.0)))
	{
		node_t* copy_node = NULL;

		if (ISNUMBER( (*node)->left ->Data, 0.0)) 
			{ copy_node = node_copy ((*node)->right);                                 assert (node_copy); } 
		if (ISNUMBER( (*node)->right->Data, 0.0)) 
			{ copy_node = node_copy ((*node)->left );                                 assert (node_copy); }

		node_dtor (*node);
		*node = copy_node;                                                            assert (*node);

		return 1;
	}

	return 0;
}
//!----------------------------------------------------------------------------------
int MulOnZero      (node_t** node)
{
	assert (node);
	assert (*node);
	assert (typeof_Data ((*node)->Data) == INSTRUCTION);
	assert (Data_instruction ((*node)->Data) == 2);
	assert ((*node)->left );
	assert ((*node)->right);

	if ((ISNUMBER( (*node)->left->Data, 0.0)) || (ISNUMBER( (*node)->right->Data, 0.0)))
	{
		Data_t* NumData = new_Data (NUMBER, 0.0);                                     assert (NumData);  
		node_t* Num_node = new_node (NumData, NULL, NULL);                            assert (Num_node);
		node_dtor (*node);

		*node = Num_node;

		return 1;
	}

	return 0;
}
//!----------------------------------------------------------------------------------
int MulOnOne       (node_t** node)
{
	assert (node);
	assert (*node);
	assert (typeof_Data ((*node)->Data) == INSTRUCTION);
	assert (Data_instruction ((*node)->Data) == 2);
	assert ((*node)->left );
	assert ((*node)->right);

	if ((ISNUMBER( (*node)->left ->Data, 1.0)) ||
		(ISNUMBER( (*node)->right->Data, 1.0)))
	{
		node_t* copy_node = NULL;

		if (ISNUMBER( (*node)->left ->Data, 1.0))
			{ copy_node = node_copy ((*node)->right);                                 assert (node_copy); } 
		if (ISNUMBER( (*node)->right->Data, 1.0)) 
			{ copy_node = node_copy ((*node)->left );                                 assert (node_copy); }

		node_dtor (*node);
		*node = copy_node;                                                            assert (*node);

		return 1;
	}

	return 0;
}
//!----------------------------------------------------------------------------------
int DegOne         (node_t** node)
{
	assert (node);
	assert (*node);
	assert (typeof_Data ((*node)->Data) == INSTRUCTION);
	assert (Data_instruction ((*node)->Data) == 6);
	assert ((*node)->left );
	assert ((*node)->right);

	if (ISNUMBER( (*node)->right->Data, 1.0))
	{
		node_t* copy_node = node_copy ((*node)->left);                                assert (copy_node);

		node_dtor (*node);
		*node = copy_node; 

		return 1;
	}

	return 0;
}
//!----------------------------------------------------------------------------------
int DegZero        (node_t** node)
{
	assert (node);
	assert (*node);
	assert (typeof_Data ((*node)->Data) == INSTRUCTION);
	assert (Data_instruction ((*node)->Data) == 6);
	assert ((*node)->left );
	assert ((*node)->right);

	if (ISNUMBER( (*node)->right->Data, 0.0))
	{
		Data_t* NumData  = new_Data (NUMBER, 1.0);                                    assert (NumData);
		node_t* Exp_node = new_node (NumData, NULL, NULL);                            assert (Exp_node);

		node_dtor (*node);
		*node = Exp_node; 

		return 1;
	}

	return 0;
}
//!==================================================================================
//! @}

//! Print_expression
//!==================================================================================
int Print_exp   (node_t* node, int old_priority)
{
	assert (node != NULL);

	int new_priority = 0;

	if (typeof_Data (node->Data) == NUMBER      ) { printf ("%lg", Data_number (node->Data)); return 1; }
	if (typeof_Data (node->Data) == VARIABLE    ) { printf ("%s",  Data_String (node->Data)); return 1; }

	if (typeof_Data (node->Data) == INSTRUCTION)
	{ 
		if (isFunction (Data_instruction (node->Data)) == 1)
		{
			if (Data_instruction (node->Data) == 60) printf ("sin" );
			if (Data_instruction (node->Data) == 61) printf ("cos" );
			if (Data_instruction (node->Data) == 62) printf ("asin");
			if (Data_instruction (node->Data) == 63) printf ("acos");
			if (Data_instruction (node->Data) == 64) printf ("ln"  );
			if (Data_instruction (node->Data) == 65) printf ("log" );
		}

		if (isOperation (Data_instruction (node->Data)) == 1)
		{
			if (Data_instruction (node->Data) == 0) new_priority = 1;
			if (Data_instruction (node->Data) == 1) new_priority = 1;
			if (Data_instruction (node->Data) == 2) new_priority = 2;
			if (Data_instruction (node->Data) == 3) new_priority = 2;
			if (Data_instruction (node->Data) == 6) new_priority = 3;
		}

		if (isFunction (Data_instruction (node->Data)) == 1)                                      printf ("(");
		if ((new_priority < old_priority) && !(isFunction (Data_instruction (node->Data)) == 1))  printf ("(");

		if (node->left  != NULL) Print_exp (node->left, new_priority);

		if (isOperation (Data_instruction (node->Data)) == 1)
		{
			if (Data_instruction (node->Data) == 0) printf ("+");
			if (Data_instruction (node->Data) == 1) printf ("-");
			if (Data_instruction (node->Data) == 2) printf ("*");
			if (Data_instruction (node->Data) == 3) printf ("/");
			if (Data_instruction (node->Data) == 6) printf ("^");
		}

		if (node->right != NULL) 
		{
			if (isFunction (Data_instruction (node->Data)) == 1) printf(", ");
			Print_exp (node->right, new_priority);
		}

		if (isFunction (Data_instruction (node->Data)) == 1)                                      printf (")");
		if ((new_priority < old_priority) && !(isFunction (Data_instruction (node->Data)) == 1))  printf (")");
	}

	return 1;
}
//!==================================================================================