#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include "Exception.hpp"

void ToUTF8(const char* aInput)
{
    FILE* Input = fopen(aInput, "rt");
    setlocale(LC_CTYPE, "Russian");
    _wsetlocale(LC_CTYPE, L"Russian_Russia.ACP");
    FILE* Output = _wfopen(L"dot\\utf8_tree.dot", L"w, ccs=UTF-8");
    if (!Input)  __THROW("Could not open %s for reading.\n", aInput);
    if (!Output) __THROW("Could not open %s for writing.\n", "dot\\utf8_tree.dot");
    fseek(Output, 0, SEEK_SET); //for dot
    
    char    InputString[512];
    wchar_t OutputString[512];
    
    while (!feof(Input))
    {
        fgets(InputString, 512, Input);
        MultiByteToWideChar(CP_ACP, 0, InputString, 512, OutputString, 512);
        fputws(OutputString, Output);    
    }
    fclose(Input);
    fclose(Output);
     
}