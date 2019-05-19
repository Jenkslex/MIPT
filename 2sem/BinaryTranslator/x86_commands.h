//!             Name                    Num      Code      Call_code
DEF_X86_CMD_ (X86_PUSH_REG       ,       8,      0x50,     x86_PushReg     (x86_cmd_t reg_num);                            )
DEF_X86_CMD_ (X86_PUSH_NUM_SMALL ,       9,      0x6A,     x86_PushNum     (int value);                                    )
DEF_X86_CMD_ (X86_POP_REG        ,      10,      0x58,     x86_PopReg      (x86_cmd_t reg_num);                            )
DEF_X86_CMD_ (X86_MOV_NUM_TO_REG ,      11,      0xB8,     x86_MovNumtoReg (int value,             x86_cmd_t reg_num    ); )
DEF_X86_CMD_ (X86_MOV_REG_TO_REG ,      12,      0x89,     x86_MovRegtoReg (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec); )
DEF_X86_CMD_ (X86_MOV_MEM_TO_REG ,      13,      0x8B,     x86_MovMemtoReg (int value,             x86_cmd_t reg_num    ); )
DEF_X86_CMD_ (X86_MOV_REG_TO_MEM ,      14,      0x89,     x86_MovRegtoMem (x86_cmd_t reg_num,     int value            ); )

DEF_X86_CMD_ (X86_ADD            ,      15,      0x01,     x86_Add         (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec); )
DEF_X86_CMD_ (X86_SUB            ,      16,      0x29,     x86_Sub         (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec); )
DEF_X86_CMD_ (X86_MUL            ,      17,      0xF7,     x86_Mul         (x86_cmd_t reg_num);                            )
DEF_X86_CMD_ (X86_DIV            ,      18,      0xF7,     x86_Div         (x86_cmd_t reg_num);                            )

DEF_X86_CMD_ (X86_CMP            ,      19,      0x39,     x86_Cmp         (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec); )
DEF_X86_CMD_ (X86_XOR            ,      20,      0x31,     x86_Xor         (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec); )

DEF_X86_CMD_ (X86_ADD_NUM        ,      21,      0x31,     x86_AddNum      (x86_cmd_t reg_num_fst, int value);             )
DEF_X86_CMD_ (X86_SUB_NUM        ,      22,      0x83,     x86_SubNum      (x86_cmd_t reg_num_fst, int value);             )


DEF_X86_CMD_ (X86_JMP            ,      23,      0xEB,     x86_Jmp         (int value);                                    )
DEF_X86_CMD_ (X86_JA             ,      24,      0x77,     x86_Ja          (int value);                                    )
DEF_X86_CMD_ (X86_JAE            ,      25,      0x73,     x86_Jae         (int value);                                    )
DEF_X86_CMD_ (X86_JB             ,      26,      0x72,     x86_Jb          (int value);                                    )
DEF_X86_CMD_ (X86_JBE            ,      27,      0x76,     x86_Jbe         (int value);                                    )
DEF_X86_CMD_ (X86_JE             ,      28,      0x74,     x86_Je          (int value);                                    )
DEF_X86_CMD_ (X86_JNE            ,      29,      0x75,     x86_Jne         (int value);                                    )

DEF_X86_CMD_ (X86_OR             ,      30,      0x09,     x86_Or          (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec); )
DEF_X86_CMD_ (X86_AND            ,      31,      0x21,     x86_And         (x86_cmd_t reg_num_fst, x86_cmd_t reg_num_sec); )

DEF_X86_CMD_ (X86_RET            ,      32,      0xC3,     x86_Ret         ();                                             )
DEF_X86_CMD_ (X86_CALL           ,      33,      0xE8,     x86_Call        (int value);                                    )
DEF_X86_CMD_ (X86_FAR_CALL       ,      34,      0xFF,     x86_Far_Call    (x86_cmd_t reg_num);                            ) 

DEF_X86_CMD_ (X86_MOV_ADD_TO_REG ,      35,      0xB8,     x86_MovAddtoReg (void* value,           x86_cmd_t reg_num    ); )
