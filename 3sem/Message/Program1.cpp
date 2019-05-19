#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void CReadTextFile (char* fileName, char** text, int* fileLength);

void sendMessage   (char* text, int textLength, char sourceName[], char pathName[]);
void sendSize      (int textLength, char sourceName[]);
void sendText      (char* text, int textLength, char pathName[]);

int main()
{
	char* text;
	int fileLength = 0;

	CReadTextFile ("Program1.cpp", &text, &fileLength);
	
	sendMessage (text, fileLength, "Program1.cpp", "Program2.cpp");

	free (text);

	return 0;
}


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


void sendMessage   (char* text, int textLength, char sourceName[], char pathName[])
{
	sendSize    (textLength, sourceName);
	sendText    (text, textLength, pathName);
}


void sendSize      (int textLength, char sourceName[])
{
	int* messageSize;
	int     shmid;
	key_t   key;

	if((key = ftok("key.txt" , 0)) < 0){
		printf("SIZE::Can\'t generate key\n");
		exit(-1);
	}
	printf("Key1 = %d\n", key);

	if((shmid = shmget(key, 1* sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if(errno != EEXIST) 
		{         
			printf("SIZE::Can't create shared memory\n");
			exit(-1);
		} 
		else 
		{
			if((shmid = shmget(key, 1* sizeof(int), 0)) < 0)
			{
				printf("SIZE::Can't find shared memory\n");
				exit(-1);
			}
		}
	}

	if((messageSize = (int*) shmat(shmid, NULL, 0)) == (int*) (-1))
	{
		printf("SIZE::Can't attach shared memory\n");
		exit(-1);
	}

	messageSize[0] = textLength;
	printf("Text length = %d\n", textLength);

	if(shmdt(messageSize) < 0)
	{
		printf("SIZE::Can't detach shared memory\n");
		exit(-1);
	}
}

void sendText      (char* text, int textLength, char pathName[])
{
	char*   sendedMessage;
	int     shmid;
	key_t   key;
	
	printf ("%d\n", textLength);

	if((key = ftok("key.txt" , 1)) < 0){
		printf("MESSAGE::Can\'t generate key\n");
		exit(-1);
	}
	
	printf("Key2 = %d\n", key);

	if((shmid = shmget(key, textLength * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if(errno != EEXIST) 
		{         
			printf("MESSAGE::Can't create shared memory\n");
			exit(-1);
		} 
		else 
		{
			if((shmid = shmget(key, textLength * sizeof(char), 0)) < 0)
			{
				printf("MESSAGE::Can't find shared memory\n");
				exit(-1);
			}
		}
	}

	if((sendedMessage = (char*) shmat(shmid, NULL, 0)) == (char*) (-1))
	{
		printf("MESSAGE::Can't attach shared memory\n");
		exit(-1);
	}

	memcpy (sendedMessage, text, textLength * sizeof (char));
	printf("Text: %s\n", sendedMessage);
	
	if(shmdt(sendedMessage) < 0)
	{
		printf("MESSAGE::Can't detach shared memory\n");
		exit(-1);
	}
}
