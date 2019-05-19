//!{=================================================================================
//! @file    main.cpp
//! @date    2015-29-03 23:15
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This program implement compile.
//!}=================================================================================

#include "stdafx.h"

#include "MyFunctions.h"
#include "Enums.h"
#include "Command.h"
#include "Builder.h"
#include "x86_Generator.h"
#include "JitPlatform.h"


//!----------------------------------------------------------------------------------
//! Constants
//!----------------------------------------------------------------------------------
const int MAXPROGSIZE = 1000;


//!----------------------------------------------------------------------------------
//! Functions
//!----------------------------------------------------------------------------------
int* ReadProgFile (char FileName[], int* MasLength);
void RecordBinFile (char FileName[], x86_command_t* x86_Program);


//! Main
//!==================================================================================
int main (int argc, _TCHAR* argv[])
{
	Builder       ListBuilder;
	x86_Generator CodeGenerator;
	JitPlatform   Platform;


	int ProgLen = 0;
	int* Program = ReadProgFile ("Code.txt", &ProgLen);

	for (int i = 0; i < ProgLen; i++) cout << Program[i] << " "; 
	cout << endl;

	vector<instr_t*>* List        = ListBuilder.Build      (Program);
	x86_command_t*    x86_Program = CodeGenerator.Generate (List   );

	RecordBinFile ("Code.bin", x86_Program);

	Platform.Run (x86_Program);

	system ("pause");
	return 0;
}
//!==================================================================================


//! Data
//! @{
//!==================================================================================
int* ReadProgFile (char FileName[], int* MasLength)
{
	assert (FileName );
	assert (MasLength);

	int temp   = 0;
	*MasLength = 0;

	int* Program = new int [MAXPROGSIZE];
	memset (Program, 0, sizeof (int) * (MAXPROGSIZE));

	ifstream myfile (FileName);

	assert (myfile.is_open());

    while (! myfile.eof() )
    {
        myfile >> temp;
		Program[(*MasLength)++] = temp;
    }
    myfile.close();

	return Program;
}
//!----------------------------------------------------------------------------------
void RecordBinFile (char FileName[], x86_command_t* x86_Program)
{
	assert (FileName);
	assert (x86_Program);

	FILE* outputfile = fopen (FileName, "wb");

	fwrite (x86_Program -> get_bin_cmd  (), sizeof (char), 
	        x86_Program -> get_cmd_size (), outputfile);

	fclose (outputfile);
}
//!==================================================================================
//! @}