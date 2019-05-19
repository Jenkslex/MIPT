//           NAME           NUMBER       TYPE              BUILDCODE        GENERATECODE
DEF_CMD_ (CMD_EXIT     ,      0,      "CMD_EXIT"     ,     BuildExit      , GenerateExit      )

DEF_CMD_ (CMD_PUSHCONST,      1,      "CMD_PUSHCONST",     BuildPushConst , GeneratePushConst )
DEF_CMD_ (CMD_PUSHREG  ,      2,      "CMD_PUSHREG"  ,     BuildPushReg   , GeneratePushReg   )
DEF_CMD_ (CMD_PUSHLOC  ,      3,      "CMD_PUSHLOC"  ,     BuildPushLoc   , GeneratePushLoc   )

DEF_CMD_ (CMD_LOADREG  ,      4,      "CMD_LOADREG"  ,     BuildLoadreg   , GenerateLoadReg   )
DEF_CMD_ (CMD_LOADLOC  ,      5,      "CMD_LOADLOC"  ,     BuildLoadLoc   , GenerateLoadLoc   )

DEF_CMD_ (CMD_ADD      ,      6,      "CMD_ADD"      ,     BuildAdd       , GenerateAdd       )
DEF_CMD_ (CMD_SUB      ,      7,      "CMD_SUB"      ,     BuildSub       , GenerateSub       )
DEF_CMD_ (CMD_MUL      ,      8,      "CMD_MUL"      ,     BuildMul       , GenerateMul       )
DEF_CMD_ (CMD_DIV      ,      9,      "CMD_DIV"      ,     BuildDiv       , GenerateDiv       )

DEF_CMD_ (CMD_JMP      ,     10,      "CMD_JMP"      ,     BuildJmp       , GenerateJmp       )
DEF_CMD_ (CMD_JA       ,     11,      "CMD_JA"       ,     BuildJa        , GenerateJa        )
DEF_CMD_ (CMD_JAE      ,     12,      "CMD_JAE"      ,     BuildJae       , GenerateJae       )
DEF_CMD_ (CMD_JB       ,     13,      "CMD_JB"       ,     BuildJb        , GenerateJb        )
DEF_CMD_ (CMD_JBE      ,     14,      "CMD_JBE"      ,     BuildJbe       , GenerateJbe       )
DEF_CMD_ (CMD_JE       ,     15,      "CMD_JE"       ,     BuildJe        , GenerateJe        )
DEF_CMD_ (CMD_JNE      ,     16,      "CMD_JNE"      ,     BuildJne       , GenerateJne       )
DEF_CMD_ (CMD_JNT      ,     17,      "CMD_JNT"      ,     BuildJnt       , GenerateJnt       )

DEF_CMD_ (CMD_CALL     ,     18,      "CMD_CALL"     ,     BuildCall      , GenerateCall      )
DEF_CMD_ (CMD_RET      ,     19,      "CMD_RET"      ,     BuildRet       , GenerateRet       )

DEF_CMD_ (CMD_EQUAL    ,     20,      "CMD_EQUAL"    ,     BuildEqual     , GenerateEqual     )
DEF_CMD_ (CMD_NOTEQ    ,     21,      "CMD_NOTEQ"    ,     BuildNoteq     , GenerateNoteq     )
DEF_CMD_ (CMD_BOREQ    ,     22,      "CMD_BOREQ"    ,     BuildBoreq     , GenerateBoreq     )
DEF_CMD_ (CMD_AOREQ    ,     23,      "CMD_AOREQ"    ,     BuildAoreq     , GenerateAoreq     )
DEF_CMD_ (CMD_BELOW    ,     24,      "CMD_BELOW"    ,     BuildBelow     , GenerateBelow     )
DEF_CMD_ (CMD_ABOVE    ,     25,      "CMD_ABOVE"    ,     BuildAbove     , GenerateAbove     )
DEF_CMD_ (CMD_AND      ,     26,      "CMD_AND"      ,     BuildAnd       , GenerateAnd       )
DEF_CMD_ (CMD_OR       ,     27,      "CMD_OR"       ,     BuildOr        , GenerateOr        )
DEF_CMD_ (CMD_NEGATE   ,     28,      "CMD_NEGATE"   ,     BuildNegate    , GenerateNegate    )

DEF_CMD_ (CMD_NEW      ,     29,      "CMD_NEW"      ,     BuildNew       , GenerateNew       )
DEF_CMD_ (CMD_CRT_FRAME,     30,      "CMD_CRT_FRAME",     BuildCrtFrame  , GenerateCrtFrame  )
DEF_CMD_ (CMD_DLT_FRAME,     31,      "CMD_DLT_FRAME",     BuildDltFrame  , GenerateDltFrame  )

DEF_CMD_ (CMD_IN_LOC   ,     32,      "CMD_IN_LOC"   ,     BuildInLoc     , GenerateInLoc     )
DEF_CMD_ (CMD_OUT_LOC  ,     33,      "CMD_OUT_LOC"  ,     BuildOutLoc    , GenerateOutLoc    )