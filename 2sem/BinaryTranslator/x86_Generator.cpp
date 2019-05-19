#include "stdafx.h"
#include "x86_Generator.h"


//! Primary settings
//! @{
//!==================================================================================
x86_Generator::x86_Generator ():
	Offsets_       {MAX_JMP_NUM},
	JmpCMDPointer_ (0)
{}
//!----------------------------------------------------------------------------------
x86_Generator::~x86_Generator ()
{}
//!==================================================================================
//! @}


//! Generating
//!==================================================================================
x86_command_t* x86_Generator::Generate (vector<instr_t*>* List)
{
	assert (List);

	vector<x86_command_t*>* Program = new vector<x86_command_t*>;    
	x86_command_t* x86Program = nullptr;

	Pass (List, Program);

	ÑalculateOffsets (List, Program);
	ClearProgram     (List, Program);

	Pass (List, Program);

	x86Program = ProgramConstructing (List, Program);

	cout << "Final program: " << endl;
	cout << *x86Program << endl;
	
	return x86Program;
}
//!==================================================================================


//! Pass
//!==================================================================================
void x86_Generator::Pass (vector<instr_t*>* List, vector<x86_command_t*>* Program)
{
	int i = 0;
	int arg_value = 0;

	JmpCMDPointer_ = 0;

	instr_t CurrCommand;
	cmd_t   CurrCMDNum  = CMD_EXIT;

	x86_command_t* Currx86Command = nullptr;

	do
	{
		CurrCMDNum = (*List)[i] -> get_cmd_num    ();
		
		if ((*List)[i] -> isJumper ()) arg_value = Offsets_[JmpCMDPointer_++];
		else                           arg_value  = (*List)[i] -> get_arg_val_fst ();

		switch (CurrCMDNum)
		{
			#define DEF_CMD_(NAME, NUMBER, TYPE, BUILDCODE, GENERATECODE)    \
				case NAME: Currx86Command = GENERATECODE (arg_value); break;
			#include "commands.h" 
			#undef DEF_CMD_
		}

		Program -> push_back (Currx86Command);

		i++;
	}
	while (CurrCMDNum != CMD_EXIT);
}
//!==================================================================================


//! HelpMethods
//! @{
//!==================================================================================
void x86_Generator::ÑalculateOffsets              (vector<instr_t*>*List, vector<x86_command_t*>* Program)
{
	int i = 0;
	int old_offset = 0, new_offset = 0,
	    start_lex  = 0, end_lex    = 0;

	JmpCMDPointer_ = 0;

	cmd_t CurrCMDNum = CMD_EXIT;

	do
	{
		CurrCMDNum = (*List)[i] -> get_cmd_num ();

		if ((*List)[i] -> isJumper ())
		{
			new_offset = 0;
			old_offset = (*List)[i] -> get_arg_val_fst ();

			if (old_offset < i)
			{
				start_lex = old_offset;
				end_lex   = i;
			}
			else
			{
				start_lex = i + 1;
				end_lex   = old_offset - 1;
			}

			for (int j = start_lex; j <= end_lex; j++)
					new_offset += (*Program)[j] -> get_cmd_size ();


			if (old_offset < i)	new_offset = -new_offset;
			
			Offsets_[JmpCMDPointer_++] = new_offset;
		}

		i++;
	}
	while (CurrCMDNum != CMD_EXIT);
}
//!----------------------------------------------------------------------------------
void x86_Generator::ClearProgram                  (vector<instr_t*>*List, vector<x86_command_t*>* Program)
{
	int i = 0;
	cmd_t   CurrCMDNum  = CMD_EXIT;

	do
	{
		CurrCMDNum = (*List)[i] -> get_cmd_num ();

		delete (*Program)[i];
		i++;
	}
	while (CurrCMDNum != CMD_EXIT);

	Program -> clear ();
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::ProgramConstructing (vector<instr_t*>*List, vector<x86_command_t*>* Program)
{
	int i = 0;
	cmd_t   CurrCMDNum  = CMD_EXIT;

	x86_command_t* x86_Program = new x86_command_t;

	do
	{
		CurrCMDNum = (*List)[i] -> get_cmd_num    ();
		
		x86_Program -> x86_prog_add ((*Program)[i]);

		i++;
	}
	while (CurrCMDNum != CMD_EXIT);

	return x86_Program;
}
//!==================================================================================
//! @}


//! Cases for generating
//! @{
//!==================================================================================
x86_command_t* x86_Generator::GenerateExit      (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GeneratePushConst (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PushNum (arg_value);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GeneratePushReg   (int arg_value)
{
	x86_command_t* Command = new x86_command_t ();

	Command -> x86_PushReg ((x86_cmd_t) arg_value);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GeneratePushLoc   (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	arg_value =  -4*arg_value;

	Command -> x86_MovMemtoReg (arg_value, X86_CMD_EAX);
	Command -> x86_PushReg                (X86_CMD_EAX);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateLoadReg   (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg ((x86_cmd_t) arg_value);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateLoadLoc   (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	arg_value =  -4*arg_value;

	Command -> x86_PopReg      (X86_CMD_EAX);
	Command -> x86_MovRegtoMem (X86_CMD_EAX, arg_value);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateAdd       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg      (X86_CMD_EAX);
	Command -> x86_PopReg      (X86_CMD_EBX);

	Command -> x86_Add         (X86_CMD_EAX, X86_CMD_EBX);

	Command -> x86_PushReg     (X86_CMD_EAX);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateSub       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg      (X86_CMD_EBX);
	Command -> x86_PopReg      (X86_CMD_EAX);

	Command -> x86_Sub         (X86_CMD_EAX, X86_CMD_EBX);

	Command -> x86_PushReg     (X86_CMD_EAX);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateMul       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg      (X86_CMD_EAX);
	Command -> x86_PopReg      (X86_CMD_EBX);

	Command -> x86_Mul         (X86_CMD_EBX);

	Command -> x86_PushReg     (X86_CMD_EAX);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateDiv       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg      (X86_CMD_EAX);
	Command -> x86_PopReg      (X86_CMD_EBX);

	Command -> x86_Xor         (X86_CMD_EDX, X86_CMD_EDX);

	Command -> x86_Div         (X86_CMD_EBX);

	Command -> x86_PushReg     (X86_CMD_EAX);

	return Command;
}
//!----------------------------------------------------------------------------------
#define GENERATEJUMPCMD(FUNCNAME, JUMPFUNC)                        \
x86_command_t* x86_Generator::FUNCNAME          (int arg_value)    \
{                                                                  \
	x86_command_t* Command = new x86_command_t;                    \
	                                                               \
	Command -> x86_PopReg  (X86_CMD_EAX);                          \
	Command -> x86_PopReg  (X86_CMD_EBX);                          \
	                                                               \
	Command -> x86_Cmp     (X86_CMD_EAX, X86_CMD_EBX);             \
	                                                               \
	Command -> JUMPFUNC    (arg_value);                            \
	                                                               \
	return Command;                                                \
}

GENERATEJUMPCMD (GenerateJe , x86_Je )
GENERATEJUMPCMD (GenerateJne, x86_Jne)
GENERATEJUMPCMD (GenerateJbe, x86_Jbe)
GENERATEJUMPCMD (GenerateJae, x86_Jae)
GENERATEJUMPCMD (GenerateJb , x86_Jb )
GENERATEJUMPCMD (GenerateJa , x86_Ja )

#undef GENERATEJUMPCMD

//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateJmp       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_Jmp     (arg_value); 

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateJnt       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg  (X86_CMD_EAX);
	Command -> x86_Xor     (X86_CMD_EBX, X86_CMD_EBX);

	Command -> x86_Cmp     (X86_CMD_EAX, X86_CMD_EBX);

	Command -> x86_Je      (arg_value); 

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateCall      (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_Call (arg_value);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateRet       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_Ret ();

	return Command;
}
//!----------------------------------------------------------------------------------
#define GENERATECMPCMD(FUNCNAME, JUMPFUNC)                         \
x86_command_t* x86_Generator::FUNCNAME          (int arg_value)    \
{                                                                  \
	x86_command_t* Command = new x86_command_t;                    \
	                                                               \
	Command -> x86_PopReg  (X86_CMD_EBX);                          \
	Command -> x86_PopReg  (X86_CMD_EAX);                          \
	                                                               \
	Command -> x86_Cmp     (X86_CMD_EAX, X86_CMD_EBX);             \
	                                                               \
	Command -> JUMPFUNC    (4);                                    \
	                                                               \
	Command -> x86_PushNum (1);                                    \
	                                                               \
	Command -> x86_Jmp     (2);                                    \
	                                                               \
	Command -> x86_PushNum (0);                                    \
	                                                               \
	return Command;                                                \
}

GENERATECMPCMD (GenerateEqual, x86_Jne)
GENERATECMPCMD (GenerateNoteq, x86_Je )
GENERATECMPCMD (GenerateBoreq, x86_Ja )
GENERATECMPCMD (GenerateAoreq, x86_Jb )
GENERATECMPCMD (GenerateBelow, x86_Jae)
GENERATECMPCMD (GenerateAbove, x86_Jbe)

#undef GENERATECMPCMD

//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateAnd       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg  (X86_CMD_EAX);
	Command -> x86_PopReg  (X86_CMD_EBX);

	Command -> x86_And     (X86_CMD_EAX, X86_CMD_EBX);

	Command -> x86_PushReg (X86_CMD_EAX); 

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateOr        (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg  (X86_CMD_EAX);
	Command -> x86_PopReg  (X86_CMD_EBX);

	Command -> x86_Or      (X86_CMD_EAX, X86_CMD_EBX);

	Command -> x86_PushReg (X86_CMD_EAX); 

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateNegate    (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PopReg  (X86_CMD_EAX);
	Command -> x86_Xor     (X86_CMD_EBX, X86_CMD_EBX);

	Command -> x86_Cmp     (X86_CMD_EAX, X86_CMD_EBX);

	Command -> x86_Jne     (4); 

	Command -> x86_PushNum (1);

	Command -> x86_Jmp     (2);

	Command -> x86_PushNum (0);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateNew       (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_SubNum      (X86_CMD_ESP, 4);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateCrtFrame  (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_PushReg     (X86_CMD_EBP);
	Command -> x86_MovRegtoReg (X86_CMD_EBP, X86_CMD_ESP);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateDltFrame  (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	Command -> x86_MovRegtoReg (X86_CMD_ESP, X86_CMD_EBP);
	Command -> x86_PopReg      (X86_CMD_EBP);

	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateInLoc     (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	arg_value = - 4 * arg_value;

	int(*funcaddress)() = &scan_num;

	Command -> x86_MovAddtoReg (funcaddress, X86_CMD_EDX);
	Command -> x86_Far_Call                 (X86_CMD_EDX);

	Command -> x86_MovRegtoMem (X86_CMD_EAX, arg_value);
 
	return Command;
}
//!----------------------------------------------------------------------------------
x86_command_t* x86_Generator::GenerateOutLoc    (int arg_value)
{
	x86_command_t* Command = new x86_command_t;

	arg_value = - 4 * arg_value;

	int(*funcaddress)(int n) = &put_num;

	Command -> x86_MovMemtoReg (arg_value  , X86_CMD_EAX);
	Command -> x86_PushReg                  (X86_CMD_EAX);

	Command -> x86_MovAddtoReg (funcaddress, X86_CMD_EDX);
	Command -> x86_Far_Call                 (X86_CMD_EDX);

	return Command;
}
//!==================================================================================
//! @}