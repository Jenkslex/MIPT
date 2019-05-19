#include "LexicalAnalyzer.h"


void LexicalAnalyzer::FillDecodeTable()
{
#define RESERVEDNAME(name, num, str)  decodeTable_[str] = name;
#include "LanguageOperators.h"
#undef  RESERVEDNAME
}