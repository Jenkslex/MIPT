#include "stdafx.h"
#include "Builder.h"


//! Primary settings
//! @{
//!==================================================================================
Builder::Builder ():
	ProgPointer_  (0),
	InstrPointer_ (0),
	CmdListNum_   {}
{}
//!----------------------------------------------------------------------------------
Builder::~Builder ()
{}
//!==================================================================================
//! @}


//! Builder
//!==================================================================================
vector<instr_t*>* Builder::Build (int* Program)
{
	assert (Program);

	ProgPointer_           = 0;
	InstrPointer_          = 0;

	cmd_t command          = CMD_EXIT;
	instr_t* CurrCommand   = nullptr;

	vector<instr_t*>* List    = new vector<instr_t*> [MAXINSTRNUM];
	vector<int>       Offsets = {};

	do
	{
		Offsets.push_back (ProgPointer_);

		command = (cmd_t) Program[ProgPointer_++];
		switch (command)
		{
			#define DEF_CMD_(NAME, NUMBER, TYPE, BUILDCODE, GENERATECODE) \
				case NAME: CurrCommand = BUILDCODE (Program); break;
			#include "commands.h"
			#undef  DEF_CMD_
		}

		InstrPointer_++;
		List -> push_back (CurrCommand);
	}
	while (command != CMD_EXIT);

	CalculateOffsets (List, Offsets);

	return List;
}
//!==================================================================================


//! Offsets calculate
//!==================================================================================
void Builder::CalculateOffsets (vector<instr_t*>* List, vector<int> Offsets)
{
	int OldOffset          = 0;
	int NewOffset          = 0;

	cmd_t command          = CMD_EXIT;

	int i = 0;
	do
	{
		command = (*List)[i] -> get_cmd_num ();

		if (isJumper (command))
		{
			OldOffset = (*List)[i] -> get_arg_val_fst ();

			for (int j = 0; j < InstrPointer_; j++) 
				if (Offsets[j] == OldOffset)
					{ NewOffset = j; break; }

			(*List)[i] -> set_arg_val_fst (NewOffset);
		}
		i++;
	}
	while (command != CMD_EXIT);
}
//!==================================================================================


//! Cases for builder
//! @{
//!==================================================================================

#define BUILDORGANIZATION(FUNCNAME, CMDNUM)                                         \
instr_t* Builder::FUNCNAME (int* Program)                                           \
{                                                                                   \
	assert (Program);                                                               \
	                                                                                \
	instr_t* CurrCommand = new instr_t;                                             \
	                                                                                \
	CurrCommand -> set_cmd_num (CMDNUM);                                            \
	CurrCommand -> set_arg_num (0);                                                 \
	                                                                                \
	return CurrCommand;                                                             \
}

BUILDORGANIZATION (BuildExit    , CMD_EXIT     )
BUILDORGANIZATION (BuildRet     , CMD_RET      )
BUILDORGANIZATION (BuildNew     , CMD_NEW      )
BUILDORGANIZATION (BuildCrtFrame, CMD_CRT_FRAME)
BUILDORGANIZATION (BuildDltFrame, CMD_DLT_FRAME)

#undef  BUILDORGANIZATION

//!----------------------------------------------------------------------------------

#define BUILDREGLOC(FUNCNAME, CMDNUM, ARGTYPE)                                      \
instr_t* Builder::FUNCNAME (int* Program)                                           \
{                                                                                   \
	assert (Program);                                                               \
	                                                                                \
	instr_t* CurrCommand = new instr_t;                                             \
	                                                                                \
	CurrCommand -> set_cmd_num (CMDNUM);                                            \
	CurrCommand -> set_arg_num (1);                                                 \
	                                                                                \
	CurrCommand -> set_arg_type_fst (ARGTYPE);                                      \
	CurrCommand -> set_arg_val_fst  (Program[ProgPointer_++]);                      \
	                                                                                \
	return CurrCommand;                                                             \
}                                             

BUILDREGLOC (BuildPushConst, CMD_PUSHCONST, NUM)
BUILDREGLOC (BuildPushReg  , CMD_PUSHREG  , REG)
BUILDREGLOC (BuildPushLoc  , CMD_PUSHLOC  , VAR)
BUILDREGLOC (BuildLoadreg  , CMD_LOADREG  , REG)
BUILDREGLOC (BuildLoadLoc  , CMD_LOADLOC  , VAR)

BUILDREGLOC (BuildInLoc    , CMD_IN_LOC   , VAR)
BUILDREGLOC (BuildOutLoc   , CMD_OUT_LOC  , VAR)

#undef BUILDORGANIZATION

//!----------------------------------------------------------------------------------

#define BUILDARITHMLOGIC(FUNCNAME, CMDNUM)                                          \
instr_t* Builder::FUNCNAME (int* Program)                                           \
{                                                                                   \
	assert (Program);                                                               \
	                                                                                \
	instr_t* CurrCommand = new instr_t;                                             \
	                                                                                \
	CurrCommand -> set_cmd_num (CMDNUM);                                            \
	CurrCommand -> set_arg_num (2);                                                 \
	                                                                                \
	CurrCommand -> set_arg_type_fst (STK);                                          \
	CurrCommand -> set_arg_type_sec (STK);                                          \
	                                                                                \
	return CurrCommand;                                                             \
} 

BUILDARITHMLOGIC (BuildAdd   , CMD_ADD   )
BUILDARITHMLOGIC (BuildSub   , CMD_SUB   )
BUILDARITHMLOGIC (BuildMul   , CMD_MUL   )
BUILDARITHMLOGIC (BuildDiv   , CMD_DIV   )

BUILDARITHMLOGIC (BuildEqual , CMD_EQUAL )
BUILDARITHMLOGIC (BuildNoteq , CMD_NOTEQ )
BUILDARITHMLOGIC (BuildBoreq , CMD_BOREQ )
BUILDARITHMLOGIC (BuildAoreq , CMD_AOREQ )
BUILDARITHMLOGIC (BuildBelow , CMD_BELOW )
BUILDARITHMLOGIC (BuildAbove , CMD_ABOVE )

BUILDARITHMLOGIC (BuildAnd   , CMD_AND   )
BUILDARITHMLOGIC (BuildOr    , CMD_OR    )
BUILDARITHMLOGIC (BuildNegate, CMD_NEGATE)

#undef BUILDARITHMLOGIC

//!----------------------------------------------------------------------------------

#define BUILDJUMP(FUNCNAME, CMDNUM)                                                 \
instr_t* Builder::FUNCNAME (int* Program)                                           \
{                                                                                   \
	assert (Program);                                                               \
	                                                                                \
	instr_t* CurrCommand = new instr_t;                                             \
	                                                                                \
	CurrCommand -> set_cmd_num (CMDNUM);                                            \
	CurrCommand -> set_arg_num (1);                                                 \
	                                                                                \
	CurrCommand -> set_arg_type_fst (ADRS);                                         \
	CurrCommand -> set_arg_val_fst  (Program[ProgPointer_++]);                      \
	                                                                                \
	return CurrCommand;                                                             \
} 

BUILDJUMP (BuildJmp , CMD_JMP )
BUILDJUMP (BuildJa  , CMD_JA  )
BUILDJUMP (BuildJae , CMD_JAE )
BUILDJUMP (BuildJb  , CMD_JB  )
BUILDJUMP (BuildJbe , CMD_JBE )
BUILDJUMP (BuildJe  , CMD_JE  )
BUILDJUMP (BuildJne , CMD_JNE )
BUILDJUMP (BuildJnt , CMD_JNT )

BUILDJUMP (BuildCall, CMD_CALL)

#undef BUILDJUMP

//!==================================================================================
//! @}

//! Help
//!==================================================================================
bool isJumper (cmd_t command)
{
	switch (command)
	{
		case CMD_JMP:	case CMD_JA:
		case CMD_JAE:	case CMD_JB:
		case CMD_JBE:	case CMD_JE:
		case CMD_JNE:	case CMD_JNT:
		case CMD_CALL:
			return true;

		default:
			return false;
	}
}
//!==================================================================================