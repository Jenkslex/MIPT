#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>


const int MAX_STRING_COUNT = 100;
const int MAX_TASKS_COUNT  = 100;
const int MAX_ARG_COUNT    =  10;
const int MAX_CMD_LEN      =  10;


struct Task
{
	int    sleepingTime;
	char*  commandName;
	char** commandArguments;
	int    commandArgumentsNumber;
};


void CReadTextFile (char* fileName, char** text, int* fileLength);
void Split (char* string, char* delimiters, char*** tokens, int* tokensCount);
void ParseTasks (char* text, Task*** tasks);
void StartPrograms (Task** tasks);
void DeleteTasks   (Task** tasks);


//! Main
//!==================================================================================
int main ()
{
	int    fileLength = 0;
	char*  text       = NULL;
	Task** tasks      = (Task**) calloc (MAX_TASKS_COUNT , sizeof (Task*));

	CReadTextFile ("PGConfigFile.txt", &text, &fileLength);

	ParseTasks (text, &tasks);

	StartPrograms (tasks);

	DeleteTasks (tasks);
	free (text);
	free (tasks);

	return 0;
}
//!==================================================================================


//! Working
//! @{
//!==================================================================================
void CReadTextFile (char* fileName, char** text, int* fileLength)
{
	FILE* file  = fopen (fileName, "r");

	fseek(file, 0, SEEK_END);
	*fileLength = ftell(file);
	fseek(file, 0, SEEK_SET);

	*text       = (char*) calloc (*fileLength, sizeof(char));

	fread (*text, sizeof (char), *fileLength, file);

	fclose(file);
}
//!==================================================================================
void Split (char* string, char* delimiters, char*** tokens, int* tokensCount)
{
	*tokensCount = 0;

	char* tempStr = NULL;

	tempStr = strtok (string, delimiters);
	(*tokens)[(*tokensCount)++] = tempStr;

	while (tempStr != NULL)
	{
		tempStr = strtok (NULL, delimiters);
		(*tokens)[(*tokensCount)++] = tempStr;
	}

	(*tokensCount)--;
}
//!==================================================================================
void ParseTasks (char* text, Task*** tasks)
{
	int stringsCount = 0,  wordsCount = 0, i = 0;
	char** strings = (char**) calloc (MAX_STRING_COUNT , sizeof (char*));
	char** words   = (char**) calloc (MAX_ARG_COUNT + 1, sizeof (char*));

	Split (text, "\n", &strings, &stringsCount);

	while (strings[i] != NULL)
	{
		(*tasks)[i] = (Task*) calloc (1, sizeof (Task));

		(*tasks)[i] -> commandArguments  = (char**) calloc (MAX_ARG_COUNT, sizeof (char*));

		Split  (strings[i + 1], " ", &(words), &(wordsCount));

		(*tasks)[i] -> sleepingTime           = atoi (words[0]);
		(*tasks)[i] -> commandName            =       words[1];
		(*tasks)[i] -> commandArgumentsNumber = wordsCount - 1;

		memcpy ((*tasks)[i] -> commandArguments, words + 1, MAX_ARG_COUNT * sizeof (char*));

		i++;
	}

	free (strings);
	free (words  );
}
//!==================================================================================
void StartPrograms (Task** tasks)
{
	int pid    = 0,
	    status = 0,
	    i      = 0;

	while (tasks[i] != NULL)
	{
		pid = fork ();

		if (pid == 0)
		{
			sleep (tasks[i] -> sleepingTime);

			printf ("\nRunning program > %s < id %d\n", tasks[i] -> commandName, getpid());
			if (tasks[i] -> commandArgumentsNumber > 1)
			{
				printf ("Program args:\n");
				for (int j = 1; j < tasks[i] -> commandArgumentsNumber; j++)
					printf ("%d> %s\n", j, tasks[i] -> commandArguments[j]);
			}

			execvp (tasks[i] -> commandName, tasks[i] -> commandArguments);

			printf ("Error running program.\n");
			exit (-1);
		}

		i++;
	}

	while (i > 0)
	{
		pid = wait (&status);
		printf ("Process %d ended with status %d\n", pid, status);
		i--;
	}
}
//!==================================================================================
void DeleteTasks   (Task** tasks)
{
	int i = 0;

	while (tasks[i] != NULL)
	{
		free (tasks[i] -> commandArguments);
		free (tasks[i]);

		i++;
	}
}
//!==================================================================================
//! @}
