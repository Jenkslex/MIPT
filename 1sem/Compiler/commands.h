//           NAME     NUMBER   ARGS     CODE
DEF_CMD_ (CMD_EXIT     ,      0,      0,     "exit"      )

DEF_CMD_ (CMD_PUSHCONST,      1,      1,     "PushConst" )
DEF_CMD_ (CMD_PUSHREG  ,      2,      1,     "PushReg"   )
DEF_CMD_ (CMD_PUSHLOC  ,      3,      1,     "PushLoc"   )

DEF_CMD_ (CMD_LOADREG  ,      4,      1,     "LoadReg"   )
DEF_CMD_ (CMD_LOADLOC  ,      5,      1,     "LoadLoc"   )


DEF_CMD_ (CMD_ADD      ,      6,      0,     "Add"       )
DEF_CMD_ (CMD_SUB      ,      7,      0,     "Sub"       )
DEF_CMD_ (CMD_MUL      ,      8,      0,     "Mul"       )
DEF_CMD_ (CMD_DIV      ,      9,      0,     "Div"       )

DEF_CMD_ (CMD_JMP      ,     10,      1,     "Jmp"       )
DEF_CMD_ (CMD_JA       ,     11,      1,     "Ja"        )
DEF_CMD_ (CMD_JAE      ,     12,      1,     "Jae"       )
DEF_CMD_ (CMD_JB       ,     13,      1,     "Jb"        )
DEF_CMD_ (CMD_JBE      ,     14,      1,     "Jbe"       )
DEF_CMD_ (CMD_JE       ,     15,      1,     "Je"        )
DEF_CMD_ (CMD_JNE      ,     16,      1,     "Jne"       )
DEF_CMD_ (CMD_JNT      ,     17,      1,     "JmpFalse"  )
DEF_CMD_ (CMD_CALL     ,     18,      1,     "Call"      )
DEF_CMD_ (CMD_RET      ,     19,      0,     "Ret"       )

DEF_CMD_ (CMD_EQUAL    ,     20,      0,     "Equal"     )
DEF_CMD_ (CMD_NOTEQ    ,     21,      0,     "Noteq"     )
DEF_CMD_ (CMD_BOREQ    ,     22,      0,     "Boreq"     )
DEF_CMD_ (CMD_AOREQ    ,     23,      0,     "Aoreq"     )
DEF_CMD_ (CMD_BELOW    ,     24,      0,     "Below"     )
DEF_CMD_ (CMD_ABOVE    ,     25,      0,     "Above"     )
DEF_CMD_ (CMD_AND      ,     26,      0,     "And"       )
DEF_CMD_ (CMD_OR       ,     27,      0,     "Or"        )
DEF_CMD_ (CMD_NEGATE   ,     28,      0,     "Negate"    )

DEF_CMD_ (CMD_NEW      ,     29,      0,     "New"       )
DEF_CMD_ (CMD_CRT_FRAME,     30,      0,     "CrtFrame"  )
DEF_CMD_ (CMD_DLT_FRAME,     31,      0,     "DltFrame"  )
DEF_CMD_ (CMD_IN_LOC   ,     32,      1,     "InLoc"     )
DEF_CMD_ (CMD_OUT_LOC  ,     33,      1,     "OutLoc"    )

