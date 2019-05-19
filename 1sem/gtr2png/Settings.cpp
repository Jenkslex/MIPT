#include "Settings.hpp"

/*

формы узлов: http://www.graphviz.org/content/node-shapes

цвета узлов: http://www.graphviz.org/content/color-names

подробнее:   http://www.graphviz.org/Documentation.php

*/

const char* InputTreeFile		= "tree.gtr";
const char* OutputPngFile		= "tree.png";

const char* StyleInstruction    = "shape=ellipse, color=\"#000000\" fillcolor=\"#d0d0d0\" style=filled";
const char* StyleNumberConstant = "shape=ellipse, color=\"#ff0000\" fillcolor=\"#ffd0d0\" style=filled";
const char* StyleStringConstant = "shape=ellipse, color=\"#ff0000\" fillcolor=\"#ffd0d0\" style=filled";
const char* StyleVariable       = "shape=ellipse, color=\"#00ff00\" fillcolor=\"#d0ffd0\" style=filled";
const char* StyleLabel          = "shape=ellipse, color=\"#00ff00\" fillcolor=\"#d0ffd0\" style=filled";
const char* StyleNewVariable    = "shape=ellipse, color=\"#0000ff\" fillcolor=\"#d0d0ff\" style=filled";
const char* StyleNewLabel       = "shape=ellipse, color=\"#0000ff\" fillcolor=\"#d0d0ff\" style=filled";
const char* StyleBlank          = "shape=ellipse, color=\"#d0d0d0\" fillcolor=\"#f0f0f0\" style=filled";
const char* StyleNewFunction    = "shape=ellipse, color=\"#e0e000\" fillcolor=\"#ffffd0\" style=filled";
const char* StyleFunction       = "shape=ellipse, color=\"#00ff00\" fillcolor=\"#d0ffd0\" style=filled";
const char* StyleParameter      = "shape=ellipse, color=\"#a0e0e0\" fillcolor=\"#d0ffff\" style=filled";
const char* StyleParameterValue = "shape=ellipse, color=\"#00ff00\" fillcolor=\"#d0ffd0\" style=filled";

const char* UND = "UNDEFINED INSTRUCTION";
const char* RES = "RESERVED INSTRUCTION";

const char* OperationNames[256] = 
{
	"+", "-", "*", "/", "%", "=", UND, UND, UND, UND,						/*0 - 9*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*10 - 19*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*20 - 29*/

	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*30 - 39*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*40 - 49*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*50 - 59*/

	"sin", "cos", "asin", "acos", "ln", "log", "pow", "sqrt", "abs", "[]",	/*60 - 69*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*70 - 79*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*80 - 89*/

	"input number", "output number", "output string", "exit", "jump",		/*90 - 94*/
	"branch", "if", "while", "function call", UND,							/*95 - 99*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*100 - 109*/
	
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*110 - 119*/
	
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*120 - 129*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*130 - 139*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*140 - 149*/

	"==", "!=", "<=", ">=", "<", ">", "&&", "||", "!", UND, 				/*150 - 159*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*160 - 169*/
	UND, UND, UND, UND, UND, UND, UND, UND, UND, UND, 						/*170 - 179*/

	"return", RES, RES, RES, RES, RES, RES, RES, RES, RES, 					/*180 - 189*/
	RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 						/*190 - 199*/
	RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 						/*200 - 209*/
	RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 						/*210 - 219*/
	RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 						/*220 - 229*/
	RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 						/*230 - 239*/
	RES, RES, RES, RES, RES, RES, RES, RES, RES, RES, 						/*240 - 149*/
	RES, RES, RES, RES, RES, RES											/*250 - 255*/
};




