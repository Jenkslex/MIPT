#include "stdafx.h"
#include "x86_command.h"


//! Primary settings
//! @{
//!==================================================================================
x86_command_t::x86_command_t ():
	bin_cmd_  {},
	cmd_size_ (0)
{}
//!----------------------------------------------------------------------------------
x86_command_t::~x86_command_t ()
{}
//!==================================================================================
//! @}


//! Primary settings
//! @{
//!==================================================================================
void x86_command_t::x86_PushReg     (x86_cmd_t reg_num)
{
	bin_cmd_[cmd_size_++] = CODE_X86_PUSH_REG | getCode (reg_num);
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_PopReg      (x86_cmd_t reg_num)
{
	bin_cmd_[cmd_size_++] = CODE_X86_POP_REG  | getCode (reg_num);
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_PushNum     (int value)
{
	bin_cmd_[cmd_size_++] = CODE_X86_PUSH_NUM_SMALL;
	bin_cmd_[cmd_size_++] = value;
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_MovNumtoReg (int value            , x86_cmd_t reg_num    )
{
	bin_cmd_[cmd_size_++] = CODE_X86_MOV_NUM_TO_REG | getCode (reg_num);
	bin_cmd_[cmd_size_++] = value;
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_MovRegtoReg (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec)
{
	bin_cmd_[cmd_size_++] = CODE_X86_MOV_REG_TO_REG;
	bin_cmd_[cmd_size_++] = 0xC0 | (getCode (reg_num_sec) << 3) | 
	                                getCode (reg_num_fst);
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_MovMemtoReg (int value            , x86_cmd_t reg_num    )
{
	bin_cmd_[cmd_size_++] = CODE_X86_MOV_MEM_TO_REG;
	bin_cmd_[cmd_size_++] = 0x40 | (getCode (reg_num) << 3) | getCode (X86_CMD_EBP);
	bin_cmd_[cmd_size_++] = value;
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_MovRegtoMem (x86_cmd_t reg_num    , int value            )
{
	bin_cmd_[cmd_size_++] = CODE_X86_MOV_REG_TO_MEM;
	bin_cmd_[cmd_size_++] = 0x40 | (getCode (reg_num) << 3) | getCode (X86_CMD_EBP);
	bin_cmd_[cmd_size_++] = value;
}
//!----------------------------------------------------------------------------------

#define ARITHMLOGICCMD(FUNCNAME, CODE)                                               \
void x86_command_t::FUNCNAME    (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec)       \
{                                                                                    \
	bin_cmd_[cmd_size_++] = CODE;                                                    \
	bin_cmd_[cmd_size_++] = 0xC0 | (getCode (reg_num_sec) << 3) |                    \
	                                getCode (reg_num_fst);                           \
}           

ARITHMLOGICCMD(x86_Add, CODE_X86_ADD) 
ARITHMLOGICCMD(x86_Sub, CODE_X86_SUB)
ARITHMLOGICCMD(x86_Cmp, CODE_X86_CMP)
ARITHMLOGICCMD(x86_Xor, CODE_X86_XOR) 
ARITHMLOGICCMD(x86_Or , CODE_X86_OR ) 
ARITHMLOGICCMD(x86_And, CODE_X86_AND) 

#undef ARITHMLOGICCMD

//!----------------------------------------------------------------------------------
void x86_command_t::x86_Mul    (x86_cmd_t reg_num)       
{                                                                                    
	bin_cmd_[cmd_size_++] = CODE_X86_MUL;                                                    
	bin_cmd_[cmd_size_++] = 0xE0 | getCode (reg_num);                           
}         
//!----------------------------------------------------------------------------------
void x86_command_t::x86_Div    (x86_cmd_t reg_num)       
{                                                                                    
	bin_cmd_[cmd_size_++] = CODE_X86_DIV;                                                    
	bin_cmd_[cmd_size_++] = 0xF0 | getCode (reg_num);                           
}  
//!----------------------------------------------------------------------------------
void x86_command_t::x86_AddNum (x86_cmd_t reg_num, int value)       
{                                                                                    
	bin_cmd_[cmd_size_++] = CODE_X86_ADD_NUM;                                                    
	bin_cmd_[cmd_size_++] = 0xC0 | getCode (reg_num);  
	bin_cmd_[cmd_size_++] = value;
}  
//!----------------------------------------------------------------------------------
void x86_command_t::x86_SubNum (x86_cmd_t reg_num, int value)       
{                                                                                    
	bin_cmd_[cmd_size_++] = CODE_X86_SUB_NUM;                                                    
	bin_cmd_[cmd_size_++] = 0xE8 | getCode (reg_num);      
	bin_cmd_[cmd_size_++] = value;
}  
//!----------------------------------------------------------------------------------

#define JMPCMD(FUNCNAME, CODE)                                                       \
void x86_command_t::FUNCNAME    (int value)                                          \
{                                                                                    \
	bin_cmd_[cmd_size_++] = CODE;                                                    \
	bin_cmd_[cmd_size_++] = value;                                                   \
}           

JMPCMD (x86_Jmp, CODE_X86_JMP)
JMPCMD (x86_Ja , CODE_X86_JA )
JMPCMD (x86_Jae, CODE_X86_JAE)
JMPCMD (x86_Jb , CODE_X86_JB )
JMPCMD (x86_Jbe, CODE_X86_JBE)
JMPCMD (x86_Je , CODE_X86_JE )
JMPCMD (x86_Jne, CODE_X86_JNE)

#undef JMPCMD

//!----------------------------------------------------------------------------------
void x86_command_t::x86_Ret      ()
{
	bin_cmd_[cmd_size_++] = CODE_X86_RET;
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_Call     (int value)
{
	bin_cmd_[cmd_size_++] = CODE_X86_CALL;

	for (int i = 0; i < 4; i++)
	{
		bin_cmd_[cmd_size_++] = value;
		value = value >> 8;
	}
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_Far_Call (x86_cmd_t reg_num)
{
	bin_cmd_[cmd_size_++] = CODE_X86_FAR_CALL;
	bin_cmd_[cmd_size_++] = 0xD0 | getCode (reg_num);
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_MovAddtoReg (void* value, x86_cmd_t reg_num)
{
	bin_cmd_[cmd_size_++] = CODE_X86_MOV_ADD_TO_REG | getCode (reg_num); 

	*(void**) (bin_cmd_ + cmd_size_) = value;
	cmd_size_ += sizeof (value);
}
//!==================================================================================
//! @}


//! Help
//!==================================================================================
void x86_command_t::Export (ostream& stream) const
{
	int temp = 0;

	for (int i = 0; i < cmd_size_; i++)
	{
		temp = (unsigned char) bin_cmd_[i];
		if (temp < 16) stream << hex << 0;
		stream << hex << temp;
	}
}
//!----------------------------------------------------------------------------------
void x86_command_t::x86_prog_add (x86_command_t* Elem)
{
	for (int i = 0; i < Elem -> cmd_size_; i++)
		bin_cmd_[cmd_size_++] = Elem -> bin_cmd_[i];
}
//!==================================================================================


//! Operators
//! @{
//!==================================================================================
ostream& operator << (ostream& stream, x86_command_t obj)
{
	obj.Export (stream);

	return stream;
}
//!==================================================================================
//! @}


//! Help
//! @{
//!==================================================================================
char getCode (x86_cmd_t reg_num)
{
	char code = 0;

	switch (reg_num)
	{
		case X86_CMD_EAX: code = 0x00; break;
		case X86_CMD_EBX: code = 0x03; break;
		case X86_CMD_ECX: code = 0x01; break;
		case X86_CMD_EDX: code = 0x02; break;
		case X86_CMD_ESI: code = 0x06; break;
		case X86_CMD_EDI: code = 0x07; break;
		case X86_CMD_ESP: code = 0x04; break;
		case X86_CMD_EBP: code = 0x05; break;
		
		default:
			assert (0);
	}

	return code;
}
//!==================================================================================
//! @}