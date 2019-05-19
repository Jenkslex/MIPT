#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

struct Data
{
	int numOne;
	int numTwo;
	long myID;
};

struct msgBufIn
{
	long         mtype;
	struct Data  data;
};

struct msgBufOut
{
	long mtype;
	int  result;
};

int CreateMessageQueue (char* pathName);

int main()
{
	int msgqid      = 0;
	int input       = 0;
	int numOne      = 0;
	int numTwo      = 0;
	char pathName[] = "Program1.cpp";

	struct msgBufIn  bufOne;
	struct msgBufOut bufTwo;

	msgqid = CreateMessageQueue (pathName);

	printf ("Enter number 1: ");
	scanf  ("%d", &numOne);
	printf ("Enter number 2: ");
	scanf  ("%d", &numTwo);

	bufOne.mtype       = 1;
	bufOne.data.myID   = getpid ();
	bufOne.data.numOne = numOne;
	bufOne.data.numTwo = numTwo;

	if((msgsnd  (msgqid, (struct msgBufIn*)  &bufOne, sizeof (struct Data), 0)) < 0)
	{
		printf ("Can\'t send message to queue\n");
		msgctl (msgqid, IPC_RMID, (struct msqid_ds*) NULL);
		exit (-1);
	}

	if ((msgrcv (msgqid, (struct msgBufOut*) &bufTwo, sizeof (int), getpid(), 0)) < 0)
	{
		printf ("Can\'t receive message from queue\n");
		msgctl (msgqid, IPC_RMID, (struct msqid_ds*) NULL);
		exit (-1);
	}

	printf("%d * %d = %d\n", numOne, numTwo, bufTwo.result);

	return 0;
}

int CreateMessageQueue (char* pathName)
{
	int key     = 0;
	int msgqid  = 0;

	if((key = ftok (pathName, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if((msgqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	return msgqid;
}
