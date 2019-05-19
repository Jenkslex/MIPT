#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

const int MAX_NUM_EVER_EVER = 10000000;

int a[3] = {};


void SemOper (int semid, int n);
void* my_thread(void* dummy);


int main()
{
	int semid;
	const char pathname[] = "Threads.cpp";
	key_t key;
	pthread_t thread_id;
	int result;

	//! Порождение семафоров
	key   = ftok(pathname , 0);
	semid = semget(key , 1 , 0666 | IPC_CREAT);
	
	
	//! Порождение второй нити
	result = pthread_create(&thread_id, (pthread_attr_t *)NULL , 
	                         my_thread, (void*) &semid);

	SemOper (semid, 1);


	for (int i = 0; i < MAX_NUM_EVER_EVER; i++)
	{
		SemOper (semid, -1);

		a[0]++;
/*
 * У вас общая переменная только a[2], т.е. работа с ней представляет собой критическую секцию.
 * Поэтому окружать семафорами следует только работу с ней, т.к. инкрементация a[0] и a[1] вполне себе может вестись параллельно.
 * В остальном нормально. Засчитано.
 */
		a[2]++;

		SemOper (semid, 1);
	}


	pthread_join(thread_id , (void **) NULL);

	for (int i = 0; i < 3; i++)
		printf ("a[%d] = %d\n", i, a[i]);

	return 0;
}


void SemOper (int semid, int n)
{
	struct sembuf mybuf;
	
	mybuf.sem_op  = n;
	mybuf.sem_flg = 0;
	mybuf.sem_num = 0;

	semop (semid , &mybuf , 1);
}

void* my_thread(void* dummy) 
{
	int semid = *((int*) dummy);

	for (int i = 0; i < MAX_NUM_EVER_EVER; i++)
	{
		SemOper (semid, -1);

		a[1]++;
		a[2]++;

		SemOper (semid, 1);
	}

	return NULL;
}
