#include <stdio.h>
#include <malloc.h>
#include <string.h>

const int MAX_STRING_LEN  = 256;
const int MAX_TOKEN_COUNT = 100;

void Split(char*  string,
           char*  delimiters,
           char*** tokens,
           int*  tokens_count);

int main ()
{
    char string[MAX_STRING_LEN] = {};
    gets (string);

    char** tokens    = (char**) calloc (MAX_TOKEN_COUNT, sizeof (char*));
    int tokens_count = 0;
    char* delimiters = " \0";

    Split (string, delimiters, &tokens, &tokens_count);

    for (int i = 0; i < tokens_count; i++)
        printf ("%s\n", tokens[i]);

    free (tokens);

    return 0;
}

void Split(char* string,
           char* delimiters,
           char*** tokens,
           int* tokens_count)
{
    *tokens_count = 0;

    char* temp_str = NULL;

    temp_str = strtok (string, delimiters);
    (*tokens)[(*tokens_count)++] = temp_str;

    while (temp_str != NULL)
    {
        temp_str = strtok (NULL, delimiters);
        (*tokens)[(*tokens_count)++] = temp_str;
    }

    (*tokens_count)--;
}
