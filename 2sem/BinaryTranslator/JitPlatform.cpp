#include "stdafx.h"
#include "JitPlatform.h"


//! Primary settings
//! @{
//!==================================================================================
JitPlatform::JitPlatform ()
{}
//!----------------------------------------------------------------------------------
JitPlatform::~JitPlatform ()
{}
//!==================================================================================
//! @}


//! Run
//!==================================================================================
void JitPlatform::Run (x86_command_t* x86_Program)
{
	int* ProgPointer = (int*) VirtualAlloc(0, x86_Program -> get_cmd_size (), 
		                                   MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	memcpy (ProgPointer,    x86_Program -> get_bin_cmd  (), 
			sizeof (char) * x86_Program -> get_cmd_size ());

	int return_value = 0;

	cout << endl << "Program is ready for running." << 
	        endl << "Press any key to start"        << endl;
	_getch ();

	_asm 
	{
		pusha

		mov edx, ProgPointer
		call edx

		mov return_value, eax

		popa
	}

	cout << "Returned value: " << dec << return_value << endl;
}
//!==================================================================================