//!         Lex Name      Number         String 
//! Basic operations
RESERVEDNAME (ADD   ,       0,            "+")
RESERVEDNAME (SUB   ,       1,            "-")
RESERVEDNAME (MUL   ,       2,            "*")
RESERVEDNAME (DIV   ,       3,            "/")
RESERVEDNAME (ASSIGN,       5,            "=")

//! Help operations
RESERVEDNAME (EXIT  ,      11,            " ")
RESERVEDNAME (ENDOP ,      12,            ";")
RESERVEDNAME (NEW   ,      13,          "int")
RESERVEDNAME (MAIN  ,      14,         "main")

//! Brackets
RESERVEDNAME (OPCBRC,      20,            "(")
RESERVEDNAME (CLCBRC,      21,            ")")
RESERVEDNAME (OPBODY,      22,            "{")
RESERVEDNAME (CLBODY,      23,            "}")
RESERVEDNAME (OPARR ,      24,            "[")
RESERVEDNAME (CLARR ,      25,            "]")

//! Language constructions
RESERVEDNAME (COMMA ,      89,            ",")
RESERVEDNAME (IN    ,      90,           ">>")
RESERVEDNAME (OUT   ,      91,           "<<")
RESERVEDNAME (IF    ,      96,           "if")
RESERVEDNAME (WHILE ,      97,        "while")
RESERVEDNAME (CALL  ,      98,            " ")

//! Logic operations
RESERVEDNAME (EQUAL ,     150,           "==")
RESERVEDNAME (NOTEQ ,     151,           "!=")
RESERVEDNAME (BOREQ ,     152,           "<=")
RESERVEDNAME (AOREQ ,     153,           ">=")
RESERVEDNAME (BELOW ,     154,            "<")
RESERVEDNAME (ABOVE ,     155,            ">")
RESERVEDNAME (AND   ,     156,           "&&")
RESERVEDNAME (OR    ,     157,           "||")
RESERVEDNAME (NEGATE,     158,            "!")

RESERVEDNAME (RETURN,     180,       "return")
