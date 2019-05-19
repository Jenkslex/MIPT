#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <pthread.h>


const int MAX_MSG_LEN  = 100;
const int SERVER_PORT  = 53800;
const int MAX_USER_NUM = 100;

struct User
{
	int socketfd;
	int number;
	char* nick;
	struct sockaddr_in usersInf;
};


int                currClientNum;
int                listener;
int                addressLen;
struct User        usersList[MAX_USER_NUM];
struct sockaddr_in serverAddress, clientAddress;
char*              reciveLine;
char*              tmp;
char*              timeStr;


int   CreateServer ();
void  send_to_all  ();
int   MakeAnswer   (char* nick, char* msg);
void* thread       (void* arg);
void  FormMessage  (char *line);


int main()
{
	reciveLine = (char*) calloc (MAX_MSG_LEN, sizeof (char));
	tmp        = (char*) calloc (MAX_MSG_LEN, sizeof (char));
/*
 * Что за 35?
 */
	timeStr    = (char*) calloc (35         , sizeof (char));
	pthread_t thread_id;

	CreateServer ();

	addressLen    = sizeof (struct sockaddr_in);
	currClientNum = 0;
/*
 * Вы решили делать также, как Алексей, учитывая MAX_USER_NUM.
 * А что, если пользователь отключится от беседы. Тогда надо уменьшить currClientNum.
 */
	while (currClientNum < MAX_USER_NUM)
	{
		if ((usersList[currClientNum].socketfd = accept (listener,
		    (struct sockaddr *) &(usersList[currClientNum].usersInf),
		    (socklen_t*) &addressLen)) < 0)
		{
			perror (NULL);
			close (listener);
			exit (1);
		}

		usersList[currClientNum].number = currClientNum;

		pthread_create (&thread_id, (pthread_attr_t *)NULL, thread,
		      (void*)(&(usersList[currClientNum].number)));

		++currClientNum;
	}
	return 0;
}

int CreateServer ()
{
	bzero (&serverAddress, sizeof (struct sockaddr_in));

	serverAddress.sin_family      = AF_INET;
	serverAddress.sin_port        = htons (SERVER_PORT);
	serverAddress.sin_addr.s_addr = htonl (INADDR_ANY);

	if ((listener = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}

	if (bind (listener, (struct sockaddr*) &serverAddress,
	    sizeof (struct sockaddr_in)) < 0)
	{
		perror (NULL);
		close (listener);
		exit (1);
	}

	if (listen (listener, MAX_USER_NUM) < 0)
	{
		perror (NULL);
		close (listener);
		exit (1);
	}

	return 0;
}

/*
 * Название в gnu стиле, которым пользуются Алёна и Алексей...
 */
void send_to_all(void)
{
	for (int i = 0; i < currClientNum; i++)
	{
		if (sendto (usersList[i].socketfd, reciveLine, MAX_MSG_LEN, 0,
		    (struct sockaddr*) &(usersList[i].usersInf),
		     sizeof (struct sockaddr_in)) < 0)
		{
			perror (NULL);
			close (usersList[i].socketfd);
			exit (1);
		}
	}
}

/*
 * Вы всегда придумываете нормальные названия для ф-й, а тут решили не заморачиваться.
 */
void* thread (void* arg)
{
	int   clientNumber           = *((int*)arg);
	usersList[clientNumber].nick = (char*) calloc (MAX_MSG_LEN, sizeof (char));

	if ((recvfrom (usersList[clientNumber].socketfd, reciveLine,
	     MAX_MSG_LEN, 0, NULL, NULL)) < 0)
	{
	   perror (NULL);
	   close (listener);
	   exit (1);
	}

	FormMessage   (reciveLine);
	strcpy        (usersList[clientNumber].nick, reciveLine);

	printf("New user :%s\n", reciveLine);

	char Admin[] = "Admin";
	reciveLine = strcat (reciveLine," join.\n" );
	MakeAnswer ((char*) Admin, reciveLine);

	send_to_all();

	while(1)
	{
		if ((recvfrom (usersList[clientNumber].socketfd, reciveLine,
		   MAX_MSG_LEN, 0, NULL, NULL)) < 0)
		{
		   perror (NULL);
		   close (listener);
/*
 * Вероятно, как раз в этом месте надо делать 
 close (usersList[clientNumber].socketfd);
 и уменьшать числа активных пользователей.
 */
		   exit (1);
		}

		FormMessage (reciveLine);
		MakeAnswer  (usersList[clientNumber].nick, reciveLine);

		send_to_all();
	}
}

/*
 * Я уже эту ф-ю третий раз вижу. Совпадение?)...
 */
void FormMessage (char *line)
{
	int i = 0;
	while (line[i] > ' ')
		++i;

	line[i] = '\0';
}

int MakeAnswer(char* nick, char* msg)
{
	bzero (tmp    , MAX_MSG_LEN);
	bzero (timeStr,          35);

	const time_t timer = time (NULL);
	timeStr            = strcpy (timeStr, ctime (&timer));

	tmp = strcpy(tmp, nick);
	tmp = strcat(tmp, (char*) ": ");
	tmp = strcat(tmp, msg);
	tmp = strcat(tmp, timeStr);
	msg = strcpy(msg, tmp);
}
