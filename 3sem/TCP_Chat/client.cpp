#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

const int MAX_MSG_LEN = 100;
const int SERVER_PORT = 53800;

int    socketfd;
struct sockaddr_in serverAddress;

int   ConnectToServer (char* IPAddress);
void* SendMessage     (void* arg);

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: ./a.out <IP address>\n");
		exit(1);
	}

	if ((socketfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror (NULL);
		exit (1);
	}

	ConnectToServer (argv[1]);
/*
 * В клиенте бы у вас никаких проблем не возникло при использовании процессов вместо потоков.
 */
	pthread_t thread_id;
	if (pthread_create (&thread_id, (pthread_attr_t *)NULL, SendMessage, NULL) < 0)
	  {
		printf("Can`t create thread\n");
		exit(-1);
	}

	char reciveLine[MAX_MSG_LEN] = {};

	while(1)
	{
		if (recvfrom (socketfd, reciveLine, MAX_MSG_LEN, 0, (struct sockaddr*) NULL, NULL) < 0)
		{
			perror (NULL);
			close (socketfd);
			exit (1);
		}
		printf("%s", reciveLine);
	}
	return 0;
}

int ConnectToServer (char* IPAddress)
{
	bzero (&serverAddress, sizeof (struct sockaddr_in));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port   = htons (SERVER_PORT);

	if (inet_aton (IPAddress, &(serverAddress.sin_addr)) == 0)
	{
		  printf ("Invalid IP address\n");
		  close (socketfd);
		  exit (1);
	}

	if (connect (socketfd, (struct sockaddr *) &serverAddress, sizeof (struct sockaddr_in)) < 0)
	{
		  printf ("Connect error\n");
		  close (socketfd);
		  exit (1);
	}
}

void* SendMessage (void* args)
{

	char sendLine[MAX_MSG_LEN] = {};

	printf ("Enter your nickname: ");
	fgets  (sendLine, MAX_MSG_LEN, stdin);

	if (sendto (socketfd, sendLine, MAX_MSG_LEN, 0,
	   (struct sockaddr*) &serverAddress, sizeof (struct sockaddr_in)) < 0)
	{
		perror (NULL);
		close (socketfd);
		exit (1);
	}

	while(1)
	{
		fgets(sendLine, MAX_MSG_LEN, stdin);

		if (sendto(socketfd, sendLine, strlen(sendLine) + 1, 0,
		   (struct sockaddr*) &serverAddress, sizeof (struct sockaddr_in)) < 0)
		{
			perror (NULL);
			close (socketfd);
			exit (1);
		}
	}
}
