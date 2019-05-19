#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

const int MAX_MSG_LEN = 1000;
const int SERVER_PORT = 53000;

int CreateClientSocket (int socketfd, struct sockaddr_in *clientAddress);
int JoinServerSocket   (int socketfd, struct sockaddr_in *serverAddress, char* IPAddress);

int SendMessage        (int socketfd, struct sockaddr_in *serverAddress);
int RecieveMessage     (int socketfd);


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: ./a.out <IP address>\n");
		exit(1);
	}

	int    socketfd;
	struct sockaddr_in serverAddress, clientAddress;

	if ((socketfd = socket (PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror (NULL);
		exit (1);
	}

	CreateClientSocket (socketfd, &clientAddress);
	JoinServerSocket   (socketfd, &serverAddress, argv[1]);

	int id = fork();

	if (id >  0) SendMessage    (socketfd, &serverAddress);
	if (id == 0) RecieveMessage (socketfd);

	return 0;
}

int CreateClientSocket (int socketfd, struct sockaddr_in *clientAddress)
{
	bzero (clientAddress, sizeof (struct sockaddr_in));

	clientAddress -> sin_family      = AF_INET;
	clientAddress -> sin_port        = htons(0);
	clientAddress -> sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind (socketfd, (struct sockaddr*) clientAddress, sizeof(struct sockaddr_in)) < 0)
	{
		perror (NULL);
		close (socketfd);
		exit (1);
	}

	return 0;
}

int JoinServerSocket   (int socketfd, struct sockaddr_in *serverAddress, char* IPAddress)
{
	bzero (serverAddress, sizeof (struct sockaddr_in));

	serverAddress -> sin_family = AF_INET;
	serverAddress -> sin_port   = htons (SERVER_PORT);

	if (inet_aton (IPAddress, &(serverAddress -> sin_addr)) == 0)
	{
		printf ("Invalid IP address\n");
		close (socketfd);
		exit (1);
	}

	return 0;
}

int SendMessage        (int socketfd, struct sockaddr_in* serverAddress)
{
	char   sendLine  [MAX_MSG_LEN] = {};

	printf("Enter your nickname:");
	scanf ("%s", sendLine);

	if (sendto (socketfd, sendLine, strlen(sendLine) + 1, 0,
		(struct sockaddr*) serverAddress, sizeof (struct sockaddr_in)) < 0)
	{
		perror(NULL);

		close(socketfd);
		exit(1);
	}

	printf ("%s\n", sendLine);

	while (1)
	{
		fgets(sendLine, MAX_MSG_LEN, stdin);

		if (sendto (socketfd, sendLine, MAX_MSG_LEN, 0,
			(struct sockaddr*) serverAddress, sizeof (struct sockaddr_in)) < 0)
		{
			perror(NULL);
			close(socketfd);
			exit(1);
		}
	}
}

int RecieveMessage     (int socketfd)
{
	char   reciveLine[MAX_MSG_LEN] = {};

	while(1)
	{
		bzero (reciveLine, MAX_MSG_LEN);

		if ((recvfrom(socketfd, reciveLine, MAX_MSG_LEN, 0,
			(struct sockaddr*) NULL, NULL)) < 0)
		{
			perror(NULL);
			close(socketfd);
			exit(1);
		}

		printf("%s", reciveLine);
	}
}
