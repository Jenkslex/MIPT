//!{=================================================================================
//! @file    main.cpp
//! @date    2015-29-03 23:15
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement compile.
//!}=================================================================================

#include "stdafx.h"

//!----------------------------------------------------------------------------------
//! Project files.
//!----------------------------------------------------------------------------------
#include "Enums.h"
#include "Data.h"
#include "BNode.h"
#include "Lexical_Analyzer.h"
#include "Func_table.h"
#include "Syntax_analyzer.h"
#include "Code_Generator.h"
#include "Assembler.h"


char* ReadFromFile (char FileName[], int* FileLength);

//! Main
//!==================================================================================
int main(int argc, _TCHAR* argv[])
{
	Lexical_Analyzer Lex_Analyzer ;
	Syntax_Analyzer  Syn_Analyzer ;
	Code_Generator   Asm_Generator;

	int   FileLength = 0;
	char* Program    = ReadFromFile ("Program.txt", &FileLength);

	Data_t** Data = Lex_Analyzer.Lexical_Analysis   (Program);
	BNode_t* root = Syn_Analyzer.Syntactic_Analysis (Data   );

	root -> Visualise ();
	
	Asm_Generator.Generate (root, "Asm.txt");

	Assembler ();

	system ("pause");
	return 0;
}
//!==================================================================================


//! Data
//!==================================================================================
char* ReadFromFile (char FileName[], int* FileLength)
{
	assert (FileName  );
	assert (FileLength);

	FILE* in = fopen (FileName, "r");                                                 assert (in);

	*FileLength = _filelength (_fileno (in));

	char* text = new char [*FileLength];                                              assert (text);
	memset (text, 0, sizeof (char) * (*FileLength));

	fread (text, *FileLength, sizeof (char), in);

	fclose (in);

	cout << "File has been readed!" << endl;

	return text;
}
//!==================================================================================