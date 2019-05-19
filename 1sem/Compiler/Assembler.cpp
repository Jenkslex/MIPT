#include "stdafx.h"
#include "Assembler.h"


//! Main
//===================================================================================
int Assembler ()
{
    FILE* in  = fopen ("Asm.txt" , "r");
    FILE* out = fopen ("Code.txt", "w");

    assert (in);
    assert (out);

    label_t Labels[MAXMARKCOUNT]  = {};
    int     Programm[MAXPROGSIZE] = {};
    int ProgLen = 0;

    Pass (Programm, &ProgLen, Labels, in);

    rewind (in);

    Pass (Programm, &ProgLen, Labels, in);

    Record (Programm, ProgLen, out);

    fclose(in);
    fclose(out);

    return 0;
}
//===================================================================================

//! Read and Write
//! @{
//===================================================================================
int Pass   (int Programm[], int* ProgLen, label_t Labels[], FILE* in)
{
    assert (in);
    assert (ProgLen);
    assert (Labels);
    assert (Programm);

    char commandstr[MAXCMDSIZE]= {};
    commands_t command = CMD_EXIT;
    int argument = 0;
    *ProgLen = 0;

    do
    {
        assert (fscanf (in, "%s", commandstr));

		cout << commandstr << endl;

        if (commandstr[strlen(commandstr) - 1] == ':') { AddLabel (Labels, commandstr, *ProgLen); continue; }
        if (commandstr[0]                      == ';') { SkipComment (in);                        continue; }

        command = ChooseCommand (commandstr);
        Programm[*ProgLen] = command;
        (*ProgLen)++;

        if (isWithArgs (command))
        {
            if      (isJumper (command)) argument = GetLabel (Labels, in);
            else                         argument = scanarg  (in);
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
int Record (int Programm[], int ProgLen, FILE* out)
{
    assert (out);
    assert (Programm);

    for (int i = 0; i < ProgLen; i++)
    {
        fprintf (out, "%d", Programm[i]);
        fprintf (out, " ");
    }

	fprintf (out, "\n");

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

    #define DEF_CMD_(name, num, args, code)  {if (!strcmp (commandstr, code)) return (commands_t) num;}
    #include "commands.h"
    #undef DEF_CMD_

    return CMD_EXIT;
}
//===================================================================================
//! @}


//! Working with labels
//! @{
//===================================================================================
int    AddLabel (label_t Labels[], char* commandstr, int ProgLen)
{
    assert (Labels);
    assert (commandstr);

	commandstr[strlen (commandstr) - 1] = '\0';

    int i = 0;
    while (Labels[i].name != nullptr) i++;

    Labels[i].name = (char*) calloc (MAXLABELSIZE, sizeof (char));
    strcpy (Labels[i].name, commandstr);

    Labels[i].pos = ProgLen;

    return 1;
}
//!----------------------------------------------------------------------------------
int    GetLabel (label_t Labels[], FILE* in)
{
    assert (Labels);
    assert (in    );

    char commandstr[MAXCMDSIZE] = {};
    fscanf (in, "%s", commandstr);

    int i = 0;
    while ((Labels[i].name != NULL) && (strcmp (Labels[i].name, commandstr) != 0))
            i++;

    return Labels[i].pos;
}
//===================================================================================
//! @}


//! Working with arguments
//! @{
//===================================================================================
int scanarg (FILE* in)
{
    assert (in);

    int argument = 0;
    char reg[MAXREGSIZE] = {};
    if (fscanf  (in,  " %d", &argument) == 0)
    {
        assert (fscanf  (in,  " %s", reg));
        argument = choosereg (reg);
    }

    return argument;
}
//!----------------------------------------------------------------------------------
int choosereg (char* reg)
{
    assert (reg);

    if (strcmp (reg, "ax") == 0) return 0;
    if (strcmp (reg, "bx") == 0) return 1;
    if (strcmp (reg, "cx") == 0) return 2;
    if (strcmp (reg, "dx") == 0) return 3;

    return 0;
}
//===================================================================================
//! @}


//! Help
//! @{
//===================================================================================
bool isWithArgs (commands_t command)
{
    #define DEF_CMD_(name, num, args, code)  if (command == name) return args;
    #include "commands.h"
    #undef DEF_CMD_

    return 0;
}
//!----------------------------------------------------------------------------------
bool isJumper (commands_t command)
{
    if ((command == CMD_JMP) || (command == CMD_CALL) ||
        (command == CMD_JA ) || (command == CMD_JAE ) ||
        (command == CMD_JB ) || (command == CMD_JBE ) ||
        (command == CMD_JE ) || (command == CMD_JNE ) ||
		(command == CMD_JNT))
        return 1;

    return 0;
}
//===================================================================================
//! @}
