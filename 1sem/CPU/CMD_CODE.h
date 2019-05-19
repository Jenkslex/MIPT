//!{=================================================================================
//! @file    CMD_CODE.h
//! @date    2014-10-25 21:02
//! @author  Ilya Kompaniets <worchun@list.ru>
//!
//! This programm include all commands of my CPU.
//!}=================================================================================

//!----------------------------------------------------------------------------------
//! Including libraries.
//!----------------------------------------------------------------------------------
#include <math.h>


//!----------------------------------------------------------------------------------
//! Defines.
//!----------------------------------------------------------------------------------
#define CONDITION(condition, ERRNUM)   if (!condition) { stkerrno = ERRNUM; return 0;}
#define VERIFIED(condition) if (!condition) return 0;


//!----------------------------------------------------------------------------------
//! Functions.
//!----------------------------------------------------------------------------------
//! Primary operations
int Push  (CPU_t* CPU, double Programm[]);
int Pushr (CPU_t* CPU, double Programm[]);
int Pop   (CPU_t* CPU, double Programm[]);

//! Reading
int ExportNumbers (stack_t* stk, double* a, double* b);

//! Arithmetic operations
int Add  (stack_t* stk);
int Sub  (stack_t* stk);
int Mult (stack_t* stk);
int Div  (stack_t* stk);
int Mod  (stack_t* stk);

//! Input and output
int OUT (CPU_t* CPU, double Programm[]);
int IN  (CPU_t* CPU, double Programm[]);

//! Jumper functions
int JMP (CPU_t* CPU, double Programm[]);
int JA  (CPU_t* CPU, double Programm[]);
int JAE (CPU_t* CPU, double Programm[]);
int JB  (CPU_t* CPU, double Programm[]);
int JBE (CPU_t* CPU, double Programm[]);
int JE  (CPU_t* CPU, double Programm[]);
int JNE (CPU_t* CPU, double Programm[]);

//! Call
int Call (CPU_t* CPU, double Programm[]);
int Ret  (CPU_t* CPU);

//! Math Functions
int SQRT (stack_t* stk);


//! Primary operations
//! @{
//===================================================================================
int Push  (CPU_t* CPU, double Programm[])
{
    assert (CPU_OK (CPU));
    CONDITION (Programm != NULL, STKERRPROG);

    double value = Programm[CPU->IP];
    CPU->IP++;

    VERIFIED (Stack_push (&(CPU->CPU_Stack), value));

    assert (CPU_OK (CPU));

    return 1;
}
//!----------------------------------------------------------------------------------
int Pushr (CPU_t* CPU, double Programm[])
{
    assert (CPU_OK (CPU));
    CONDITION (Programm != NULL, STKERRPROG);

    int reg = Programm[CPU->IP];
    CPU->IP++;

    double value = 0;

    value = CPU->Regs[reg];

    VERIFIED (Stack_push (&(CPU->CPU_Stack), value));

    assert (CPU_OK (CPU));

    return 1;
}
//!----------------------------------------------------------------------------------
int Pop (CPU_t* CPU, double Programm[])
{
    assert (CPU_OK (CPU));
    CONDITION (Programm != NULL, STKERRPROG);

    int reg = Programm[CPU->IP];
    CPU->IP++;

    double value = 0;
    VERIFIED (Stack_pop (&(CPU->CPU_Stack), &value));

    CPU->Regs[reg] = value;

    assert (CPU_OK (CPU));

    return 1;
}
//===================================================================================
//! @}


//! Reading
//! @{
//===================================================================================
int ExportNumbers (stack_t* stk, double* a, double* b)
{
    assert (Stack_OK (stk));
    CONDITION (stk->top >= 2,  STKERRFEWARG);

    double value = 0;

    VERIFIED (Stack_pop (stk, &value));
    *a = value;

    VERIFIED (Stack_pop (stk, &value));
    *b = value;

    assert (Stack_OK (stk));

    return 1;
}
//===================================================================================
//! @}


//! Arithmetic operations
//! @{
//===================================================================================
#define ARITHMCOM(name, op, addcond, ERRNUM)  \
int name  (stack_t* stk)                      \
{                                             \
    assert (Stack_OK (stk));                  \
                                              \
    double a = 0, b = 0;                      \
    VERIFIED  (ExportNumbers (stk, &a, &b));  \
    CONDITION (addcond, ERRNUM);              \
    double value = op;                        \
    VERIFIED  (Stack_push (stk, value));      \
                                              \
    assert (Stack_OK (stk));                  \
                                              \
    return 1;                                 \
}
//!----------------------------------------------------------------------------------
ARITHMCOM(Add,  a + b, 1, 0)
ARITHMCOM(Sub,  a - b, 1, 0)
ARITHMCOM(Mult, a * b, 1, 0)
ARITHMCOM(Div,  a / b, b != 0, STKERRDIVIDE)
ARITHMCOM(Mod,  (int) a % (int) b, b != 0, STKERRDIVIDE)
//===================================================================================
//! @}


//! Input and output
//===================================================================================
int OUT (CPU_t* CPU, double Programm[])
{
    assert (CPU_OK (CPU));
    assert ((Programm != NULL));

    int reg = Programm[CPU->IP];
    CPU->IP++;

    VERIFIED (printf ("%lg\n", CPU->Regs[reg]));

    assert (CPU_OK (CPU));

    return 1;
}
//!----------------------------------------------------------------------------------
int IN  (CPU_t* CPU, double Programm[])
{
    assert (CPU_OK (CPU));
    assert ((Programm != NULL));

    int reg = Programm[CPU->IP];
    CPU->IP++;

    double value = 0;

    VERIFIED (scanf ("%lg", &value));

    CPU->Regs[reg] = value;

    assert (CPU_OK (CPU));

    return 1;
}
//===================================================================================


//! Jumper functions
//! @{
//===================================================================================
int JMP (CPU_t* CPU, double Programm[])
{
    assert (CPU_OK (CPU));
    assert ((Programm != NULL));

    int value = Programm[CPU->IP];
    CPU->IP = value;

    assert (CPU_OK (CPU));

    return 1;
}
//!----------------------------------------------------------------------------------
#define JUMPERFUNC(name, addition)                         \
int name  (CPU_t* CPU, double Programm[])                  \
{                                                          \
    assert (CPU_OK (CPU));                                 \
    assert ((Programm != NULL));                           \
                                                           \
    double a = 0, b = 0;                                   \
    VERIFIED (ExportNumbers (&(CPU->CPU_Stack), &a, &b));  \
                                                           \
    if (b addition a) JMP (CPU, Programm);                 \
                                                           \
    else CPU->IP++;                                        \
                                                           \
    assert (CPU_OK (CPU));                                 \
                                                           \
    return 1;                                              \
}
//!----------------------------------------------------------------------------------
JUMPERFUNC (JA,  >)
JUMPERFUNC (JAE, >=)
JUMPERFUNC (JB,  <)
JUMPERFUNC (JBE, <=)
JUMPERFUNC (JE,  ==)
JUMPERFUNC (JNE, !=)
//===================================================================================
//! @}


//! Call
//! @{
//===================================================================================
int Call (CPU_t* CPU, double Programm[])
{
    assert (CPU_OK (CPU));
    assert ((Programm != NULL));

    int value = CPU->IP;

    VERIFIED (Stack_push (&(CPU->Call_Stack), value + 1));
    JMP (CPU, Programm);

    assert (CPU_OK (CPU));

    return 1;
}
//!----------------------------------------------------------------------------------
int Ret (CPU_t* CPU)
{
    assert (CPU_OK (CPU));

    double value = 0;

    if (!Stack_Empty (&(CPU->Call_Stack)))
    {
        VERIFIED (Stack_pop (&(CPU->Call_Stack), &value));
        CPU->IP = value;
    }

    assert (CPU_OK (CPU));

    return 1;
}
//===================================================================================
//! @}


//! Math functions
//===================================================================================
int SQRT (stack_t* stk)
{
    assert (Stack_OK (stk));

    double value = 0;
    VERIFIED (Stack_pop (stk, &value));

    value = sqrt(value);
    VERIFIED (Stack_push (stk, value));

    assert (Stack_OK (stk));

    return 1;
}
//===================================================================================

#undef VERIFIED
