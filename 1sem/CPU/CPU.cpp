//!{=================================================================================
//! @file    CPU.cpp
//! @date    2014-10-01 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This programm realise stack CPU.
//!}=================================================================================

//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "Stack.h"


//!----------------------------------------------------------------------------------
//! Defines.
//!----------------------------------------------------------------------------------
#define CONDITION(condition, ERRNUM)   if (!condition) { stkerrno = ERRNUM; return 0;}
#define ASSERT(condition) if (!condition) {CPU_Dump (CPU); perror ("ER"); assert(condition);}


//!----------------------------------------------------------------------------------
//! Enums
//!----------------------------------------------------------------------------------
enum commands_t
{
    #define DEF_CMD_(name, num, args, code)  CMD_##name = num,
    #include "commands.h"
    #undef DEF_CMD_
    CMD_MAX
};


//!----------------------------------------------------------------------------------
//! Constants
//!----------------------------------------------------------------------------------
const int MAXMARKCOUNT = 10;
const int MAXPROGSIZE  = 400;
const int MAXREGNUM    = 10;


//!----------------------------------------------------------------------------------
//! Structs
//!----------------------------------------------------------------------------------
struct CPU_t
{
    stack_t CPU_Stack;
    stack_t Call_Stack;

    double Regs[MAXREGNUM];
    int IP;
};


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Prepare
int ReadProgramm (double Programm[], FILE* in);

//! ProcessCommands
int ProcessCommands (CPU_t* CPU, double Programm[]);
commands_t ReadCommand (double Programm[]);

//! CPU
int CPU_ctor (CPU_t* CPU);
int CPU_dtor (CPU_t* CPU);
int CPU_OK   (CPU_t* CPU);
int CPU_Dump (CPU_t* CPU);

//! Help
int isWithArgs (int command);

//! DIE!!!
int die (CPU_t* CPU, char* FuncName, const char* file, const int line);

//! Commands
#include "CMD_CODE.h"


//! Main
//===================================================================================
int main ()
{
    FILE* in  = fopen ("Code.txt", "r");
    assert (in);

    double Programm[MAXPROGSIZE] = {};
    ReadProgramm (Programm, in);

    CPU_t CPU = {};
    CPU_ctor (&CPU);

    ProcessCommands (&CPU, Programm);

    CPU_dtor (&CPU);

    return 0;
}
//===================================================================================


//! Reading
//===================================================================================
int ReadProgramm (double Programm[], FILE* in)
{
    assert (in);
    assert (Programm);

    double command = 0;
    double arg = 0;
    int ProgLen = 0;

    do
    {
        fscanf (in, "%lg", &command);
        Programm[ProgLen] = (int) command;
        ProgLen++;
        if (isWithArgs ((int) command))
        {
            fscanf (in, "%lg", &arg);
            Programm[ProgLen] = arg;
            ProgLen++;
        }
    }
    while ((int) command != CMD_EXIT);

    return 0;
}
//===================================================================================


//! ProcessCommands
//===================================================================================
int ProcessCommands (CPU_t* CPU, double Programm[])
{
    CPU_OK (CPU);
    assert (Programm);

    commands_t command = CMD_EXIT;
    do
    {
        command = (commands_t) Programm[CPU->IP];
        CPU->IP++;

        switch (command)
        {
            #define VERIFIED(code) if (!(code))       \
                { die (CPU, #code, __FILE__, __LINE__); return 0; }
            #define DEF_CMD_(name, num, args, code)   \
                case CMD_##name: VERIFIED(code); break;
            #include "commands.h"
            #undef DEF_CMD_
            #undef VERIFIED

            default: return 0;
        }
    }
    while (command != CMD_EXIT);

    return 1;
}
//===================================================================================


//! CPU
//! @{
//===================================================================================
int CPU_ctor (CPU_t* CPU)
{
    ASSERT (CPU);

    ASSERT (Stack_ctor (&(CPU->CPU_Stack)));
    ASSERT (Stack_ctor (&(CPU->Call_Stack)));

    for (int i = 0; i < MAXREGNUM; i++) CPU->Regs[i] = 0;
    CPU->IP = 0;

    ASSERT (CPU_OK (CPU));

    return 1;
}
//!----------------------------------------------------------------------------------
int CPU_dtor (CPU_t* CPU)
{
    ASSERT (CPU_OK (CPU));

    ASSERT (Stack_dtor (&(CPU->CPU_Stack)));
    ASSERT (Stack_dtor (&(CPU->Call_Stack)));

    for (int i = 0; i < MAXREGNUM; i++) CPU->Regs[i] = -1;
    CPU->IP = -1;

    return 1;
}
//!----------------------------------------------------------------------------------
int CPU_OK   (CPU_t* CPU)
{
    if (CPU == NULL) return 0;
    if (Stack_OK (&(CPU->CPU_Stack))  == 0) return 0;
    if (Stack_OK (&(CPU->Call_Stack)) == 0) return 0;

    return 1;
}
//!----------------------------------------------------------------------------------
int CPU_Dump (CPU_t* CPU)
{
    printf ("\nCPU_t [0x%p]\n", CPU);

    printf ("CPU_Stack\n");
    Stack_Dump (&(CPU->CPU_Stack));

    printf ("Call_Stack\n");
    Stack_Dump (&(CPU->Call_Stack));

    for (int i = 0; i < MAXREGNUM; i++)
        printf ("Reg[%d] = %lg\n", i, CPU->Regs[i]);
    printf ("IP = %d\n", CPU->IP);

    return 0;
}
//===================================================================================
//! @}


//! Help
//===================================================================================
int isWithArgs (int command)
{
    #define DEF_CMD_(name, num, args, code)  if (command == num) return args;
    #include "commands.h"
    #undef DEF_CMD_

    return 0;
}
//===================================================================================


//! DIE!!!
//===================================================================================
int die (CPU_t* CPU, char* FuncName, const char* file, int line)
{
    printf ("Error: programm was failed with status %d\n", stkerrno);
    printf ("In function %s\n", FuncName);
    printf ("%s\n", stkstrerr (stkerrno));
    printf ("File: %s\n", file);
    printf ("Line: %d\n", line);
    CPU_Dump (CPU);

    assert (0);
    return 0;
}
//===================================================================================
