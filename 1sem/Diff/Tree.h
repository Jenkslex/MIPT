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
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "Data.h"


//!----------------------------------------------------------------------------------
//! Structs.
//!----------------------------------------------------------------------------------
struct node_t
{
    Data_t* Data;
    node_t* left;
    node_t* right;
};


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Primary settings
int node_ctor     (node_t* node, Data_t* Data, node_t* left, node_t* right);
node_t* new_node  (Data_t* Data, node_t* left, node_t* right);
int node_dtor     (node_t* node);
node_t* node_copy (node_t* node);

//! Diagnostics
int node_OK      (node_t* node);
int node_Dump    (node_t* node, int deep = 0);
