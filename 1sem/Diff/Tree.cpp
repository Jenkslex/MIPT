#include "Tree.h"
#include "stdafx.h"

//!----------------------------------------------------------------------------------
//! Definitions.
//!----------------------------------------------------------------------------------
#define TABS for (int tempcounter = 0; tempcounter < deep; tempcounter++)  \
            printf ("           ");


//! Primary settings
//! @{
//!==================================================================================
int node_ctor     (node_t* node, Data_t* Data, node_t* left, node_t* right)
{
    assert (node  != NULL);
    assert (Data  != NULL);

    node->Data  = Data;
    node->left  = left;
    node->right = right;

    assert (node_OK (node));

    return 1;
}
//!----------------------------------------------------------------------------------
node_t* new_node  (Data_t* Data, node_t* left, node_t* right)
{
    assert (Data != NULL);

    node_t* new_node = (node_t*) calloc (1, sizeof (node_t));                         assert (new_node != NULL);

    node_ctor (new_node, Data, left, right);

	assert (node_OK (new_node));

    return new_node;
}
//!----------------------------------------------------------------------------------
int node_dtor     (node_t* node)
{
    assert (node != NULL);

    if (node->left  != NULL) node_dtor (node->left);
    if (node->right != NULL) node_dtor (node->right);

	Data_dtor  (node->Data);

    node->Data  = NULL;
    node->left  = NULL;
    node->right = NULL;

    free (node);

    return 1;
}
//!----------------------------------------------------------------------------------
node_t* node_copy (node_t* node)
{
	assert (node != NULL);

	node_t* copy_left_node  = NULL;
	node_t* copy_right_node = NULL;

	if (node->left  != NULL) { copy_left_node  = node_copy (node->left );      assert (copy_left_node ); }
	if (node->right != NULL) { copy_right_node = node_copy (node->right);      assert (copy_right_node); }

	Data_t* copy_Data = Data_copy (node->Data);                                assert (copy_Data);

	node_t* copy_node = new_node (copy_Data, copy_left_node, copy_right_node); assert (copy_node);

	return copy_node;
}
//!==================================================================================
//! @}


//! Diagnostics
//! @{
//!==================================================================================
int node_OK   (node_t* node)
{
    if (node       == NULL) return 0;
    if (node->Data == NULL) return 0;

    return 1;
}
//!----------------------------------------------------------------------------------
int node_Dump (node_t* node, int deep)
{
    assert (node != NULL);

	if (node->left  != NULL) node_Dump (node->left,  deep + 1);

    TABS printf ("------node--Dump------\n");

    TABS printf ("node [0x%p]\n", node);
    TABS printf ("Data [0x%p]\n", node->Data);
	if (node->Data != NULL) { TABS PrintData (node->Data); }
         printf ("\n");

    TABS printf ("left  = 0x%p\n", node->left);
    TABS printf ("right = 0x%p\n", node->right);

    TABS printf ("----------------------\n");

    if (node->right != NULL) node_Dump (node->right, deep + 1);

    return 1;
}
//!==================================================================================
//! @}