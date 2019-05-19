//!     Lex Name    Number    String 
//! Basic operations
RESERVEDNAME(Add   ,  0,       "+")
RESERVEDNAME(Sub   ,  1,       "-")
RESERVEDNAME(Mul   ,  2,       "*")
RESERVEDNAME(Div   ,  3,       "/")
RESERVEDNAME(Assign,  5,       "=")

//! Help operations
RESERVEDNAME(Exit  , 11,       " ")
RESERVEDNAME(EndOp , 12,       ";")
RESERVEDNAME(New   , 13,     "int")
RESERVEDNAME(Main  , 14,    "main")

//! Brackets
RESERVEDNAME(OpCBrc, 20,       "(")
RESERVEDNAME(ClCBrc, 21,       ")")
RESERVEDNAME(OpBody, 22,       "{")
RESERVEDNAME(ClBody, 23,       "}")

//! Language constructions
RESERVEDNAME(Comma , 89,       ",")
RESERVEDNAME(In    , 90,      ">>")
RESERVEDNAME(Out   , 91,      "<<")
RESERVEDNAME(If    , 96,      "if")
RESERVEDNAME(While , 97,   "while")
RESERVEDNAME(Call  , 98,       " ")

//! Logic operations
RESERVEDNAME(Equal , 150,     "==")
RESERVEDNAME(NotEq , 151,     "!=")
RESERVEDNAME(BOrEq , 152,     "<=")
RESERVEDNAME(AOrEq , 153,     ">=")
RESERVEDNAME(Below , 154,      "<")
RESERVEDNAME(Above , 155,      ">")
RESERVEDNAME(And   , 156,     "&&")
RESERVEDNAME(Or    , 157,     "||")
RESERVEDNAME(Negate, 158,      "!")

RESERVEDNAME(Return, 180, "return")