#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void RecieveMessage   (char** text, char sourceName[], char pathName[]);
int  RecieveSize      (char sourceName[]);
void RecieveText      (char** text, int textLength, char pathName[]);

int main()
{
	char* text;

	RecieveMessage (&text, "Program1.cpp", "Program2.cpp");

	printf ("%s", text);

	free (text);

	return 0;
}



void RecieveMessage   (char** text, char sourceName[], char pathName[])
{
	int messageSize = RecieveSize (sourceName);

	RecieveText    (text, messageSize, pathName);
}


int  RecieveSize      (char sourceName[])
{
	int size;
	int* messageSize;
	int     shmid;
	key_t   key;

	if((key = ftok("key.txt" , 0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}
	printf("Key1 = %d\n", key);

	if((shmid = shmget(key, 1* sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if(errno != EEXIST) 
		{         
			printf("Can't create shared memory\n");
			exit(-1);
		} 
		else 
		{
			if((shmid = shmget(key, 1* sizeof(int), 0)) < 0)
			{
				printf("Can't find shared memory\n");
				exit(-1);
			}
		}
	}

	if((messageSize = (int*) shmat(shmid, NULL, 0)) == (int*) (-1))
	{
		printf("Can't attach shared memory\n");
		exit(-1);
	}

	size = messageSize[0];
	printf("Text size = %d\n", size);

	if(shmdt(messageSize) < 0)
	{
		printf("Can't detach shared memory\n");
		exit(-1);
	}
	
	return size;
}

void RecieveText      (char** text, int textLength, char pathName[])
{
	char*   sendedMessage;
	int     shmid;
	key_t   key;

	if((key = ftok("key.txt" , 1)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}
	printf("Key2 = %d\n", key);

	if((shmid = shmget(key, textLength * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if(errno != EEXIST) 
		{         
			printf("Can't create shared memory\n");
			exit(-1);
		} 
		else 
		{
			if((shmid = shmget(key, textLength * sizeof(char), 0)) < 0)
			{
				printf("Can't find shared memory\n");
				exit(-1);
			}
		}
	}

	if((sendedMessage = (char*) shmat(shmid, NULL, 0)) == (char*) (-1))
	{
		printf("Can't attach shared memory\n");
		exit(-1);
	}
	
	(*text) = (char*) calloc (textLength, sizeof (char));
	memcpy ((*text), sendedMessage, textLength * sizeof (char));

	if(shmdt(sendedMessage) < 0)
	{
		printf("Can't detach shared memory\n");
		exit(-1);
	}
}
