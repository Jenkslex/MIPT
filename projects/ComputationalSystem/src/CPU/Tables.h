#pragma once

enum CPU_Command
{
#define DEF_CMD_(name, num, args, code)  name = num,
#include "../CPU_commands.h"
#undef DEF_CMD_

	CMD_ERROR = -1
};