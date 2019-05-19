//!{=================================================================================
//! @file    Asm.cpp
//! @date    2014-10-22 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This programm realise simple example of interpreter.
//!}=================================================================================

//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <assert.h>


//!----------------------------------------------------------------------------------
//! Defines.
//!----------------------------------------------------------------------------------
#define VERIFIED(condition) if (!condition) return 0;


//! Enums
//!----------------------------------------------------------------------------------
enum commands_t
{
    #define DEF_CMD_(name, num, args, code)  CMD_##name = num,
    #include "commands.h"
    #undef DEF_CMD_
    CMD_MAX
};



//! Constants
//!----------------------------------------------------------------------------------
const int MAXMARKCOUNT = 10;
const int MAXPROGSIZE  = 400;
const int MAXCMDSIZE   = 30;
const int MAXREGSIZE   = 10;


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Read and Write
int Pass (double Marks[], double Programm[], int* ProgLen, FILE* in);
int SkipComment (FILE* in);
int Record (double Programm[], int ProgLen, FILE* out);

//! Working with commands
commands_t ChooseCommand (const char* commandstr);

//! Working with arguments
double scanarg (FILE* in);
double choosereg (char* reg);

//! Help
int isWithArgs (commands_t command);
int isJumper   (commands_t command);


//! Main
//===================================================================================
int main ()
{
    FILE* in  = fopen ("Programm.txt", "r");
    FILE* out = fopen ("Code.txt",     "w"); // wb fread потом

    assert (in);
    assert (out);

    double Marks[MAXMARKCOUNT] = {};
    double Programm[MAXPROGSIZE] = {};
    int ProgLen = 0;

    Pass (Marks, Programm, &ProgLen, in);

    rewind (in);

    Pass (Marks, Programm, &ProgLen, in);

    Record (Programm, ProgLen, out);

    fclose(in);
    fclose(out);

    return 0;
}
//===================================================================================

//! Read and Write
//! @{
//===================================================================================
int Pass (double Marks[], double Programm[], int* ProgLen, FILE* in)
{
    assert (in);
    assert (ProgLen);
    assert (Marks);
    assert (Programm);

    char commandstr[MAXCMDSIZE]= {};
    commands_t command = CMD_EXIT;
    double argument = 0;
    *ProgLen = 0;
    do
    {
        assert (fscanf (in, "%s", commandstr));

        if (commandstr[0] == ':') { Marks[ commandstr[1] - '0'] = *ProgLen; continue; }
        if (commandstr[0] == ';') { SkipComment (in);                       continue; }

        // TODO: finding first non-space character
        // int i = 0;
        // while (!(isspace(commandstr[i++]));

        command = ChooseCommand (commandstr);
        Programm[*ProgLen] = command;
        (*ProgLen)++;

        // TODO: Check below the constant

        if (isWithArgs (command))
        {
            argument = scanarg (in);
            if (isJumper (command)) argument = Marks[(int) argument];
            Programm[*ProgLen] = argument;
            (*ProgLen)++;
        }
    }
    while (command != CMD_EXIT);

    return 0;
}
//!----------------------------------------------------------------------------------
int SkipComment (FILE* in)
{
    assert (in);

    char temp = 0;
    while (temp != '\n') assert (fscanf (in, "%c", &temp));
    return 0;
}
//!----------------------------------------------------------------------------------
int Record (double Programm[], int ProgLen, FILE* out)
{
    assert (out);
    assert (Programm);

    for (int i = 0; i < ProgLen; i++)
    {
        fprintf (out, "%lg", Programm[i]);
        fprintf (out, " ");
    }
    //assert (fwrite (Programm, sizeof (double), ProgLen, out));
    return 0;
}
//===================================================================================
//! @}


//! Working with commands
//! @{
//===================================================================================
commands_t ChooseCommand (const char* commandstr)
{
    assert (commandstr);

    #define DEF_CMD_(name, num, args, code)  {if (!strcmp (commandstr, #name)) return (commands_t) num;}
    #include "commands.h"
    #undef DEF_CMD_

    return CMD_EXIT;
}
//===================================================================================
//! @}


//! Working with arguments
//! @{
//===================================================================================
double scanarg (FILE* in)
{
    assert (in);

    double argument = 0;
    char reg[MAXREGSIZE] = {};
    if (fscanf  (in,  " %lg", &argument) == 0)
    {
        assert (fscanf  (in,  " %s", reg));
        argument = choosereg (reg);
    }

    return argument;
}
//!----------------------------------------------------------------------------------
double choosereg (char* reg)
{
    assert (reg);

    if (strcmp (reg, "AX") == 0) return 1;
    if (strcmp (reg, "BX") == 0) return 2;
    if (strcmp (reg, "CX") == 0) return 3;
    if (strcmp (reg, "DX") == 0) return 4;
    if (strcmp (reg, "EX") == 0) return 5;
    if (strcmp (reg, "FX") == 0) return 6;

    return 0;
}
//===================================================================================
//! @}


//! Help
//! @{
//===================================================================================
int isWithArgs (commands_t command)
{
    #define DEF_CMD_(name, num, args, code)  if (command == CMD_##name) return args;
    #include "commands.h"
    #undef DEF_CMD_

    return 0;
}
//!----------------------------------------------------------------------------------
int isJumper (commands_t command)
{
    if ((command == CMD_JMP) || (command == CMD_CALL) ||
        (command == CMD_JA)  || (command == CMD_JAE)  ||
        (command == CMD_JB)  || (command == CMD_JBE)  ||
        (command == CMD_JE)  || (command == CMD_JNE))
        return 1;

    return 0;
}
//===================================================================================
//! @}
