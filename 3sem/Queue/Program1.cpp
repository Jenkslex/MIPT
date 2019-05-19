#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

const int MAX_THREAD_NUM = 10;

struct Data
{
	int numOne;
	int numTwo;
	long myID;
};

struct MsgBufIn
{
	long         mtype;
	struct Data  data;
};

struct MsgBufOut
{
	long mtype;
	int  result;
};

struct ThreadArguments
{
	struct MsgBufIn bufIn;
	int             threadNum;
};

int msgqid = 0;
int isThreadWorking[MAX_THREAD_NUM] = {};

int  CreateMessageQueue       (char* pathName);
int  GetNumOfNonWorkingThread ();
int  ConstrictingArguments    (int threadNum, struct MsgBufIn* bufIn,
	                             struct ThreadArguments* arguments);
void* myThread                (void* dummy);
void Exit                     (int nsig);

int main()
{
	pthread_t               thread_id;
	int                     result;
	char                    pathName[] = "Program1.cpp";
	int                     numOne     = 0;
	int                     numTwo     = 0;
	int                     freeThread = 0;
	struct MsgBufIn         bufIn;
	struct ThreadArguments  arguments[MAX_THREAD_NUM] = {};

	signal (SIGINT, Exit);

	msgqid = CreateMessageQueue (pathName);

	while (1)
	{
		if ((msgrcv (msgqid, (struct MsgBufIn*) &bufIn, sizeof (struct Data), 1, 0)) < 0)
		{
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}

		while ((freeThread = GetNumOfNonWorkingThread ()) == -1)
			sleep (1);

		isThreadWorking[freeThread] = 1;
		ConstrictingArguments (freeThread, &bufIn, arguments);

		if (result = pthread_create (&thread_id, (pthread_attr_t *) NULL, myThread, arguments + freeThread))
		{
			printf("Can`t create thread, returned value = %d\n" , result);
			exit(-1);
		}
	}

	return 0;
}


int CreateMessageQueue (char* pathName)
{
	int key    = 0;
	int msgqid = 0;

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

int GetNumOfNonWorkingThread ()
{
	int freeThread = -1;

	for (int i = 0; i < MAX_THREAD_NUM; i++)
		if (isThreadWorking[i] == 0)
		{
			freeThread = i;
			break;
		}

		printf ("frre Thread is %d\n", freeThread);

	return freeThread;
}

int  ConstrictingArguments    (int threadNum, struct MsgBufIn* bufIn,
	                             struct ThreadArguments* arguments)
{
	arguments[threadNum].threadNum = threadNum;
	arguments[threadNum].bufIn     = *bufIn;
}


void* myThread (void* dummy)
{
	struct ThreadArguments* arguments = (struct ThreadArguments*) dummy;
	struct MsgBufOut        bufOut;

	bufOut.mtype  = arguments -> bufIn.data.myID;
	bufOut.result = arguments -> bufIn.data.numOne *
	                arguments -> bufIn.data.numTwo;

	if(msgsnd (msgqid, (struct MsgBufOut*) &bufOut, sizeof(int), 0) < 0)
	{
	  printf("Can\'t send message to queue\n");
	  msgctl(msgqid, IPC_RMID, (struct msqid_ds*) NULL);
	  exit(-1);
	}

	isThreadWorking [arguments -> threadNum] = 0;

	return NULL;
}

void Exit (int nsig)
{
	msgctl(msgqid, IPC_RMID, (struct msqid_ds*)NULL);

	exit (-1);
}
