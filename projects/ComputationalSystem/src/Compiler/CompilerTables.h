#pragma once

enum Operator
{
#define RESERVEDNAME(name, num, str)  name = num,
#include "LanguageOperators.h"
#undef  RESERVEDNAME

	UnknownOperator
};