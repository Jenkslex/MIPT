#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

const int MAX_MSG_LEN  = 1000;
const int SERVER_PORT  = 53000;
const int MAX_USER_NUM = 100;


struct User
{
	char* nick;
	struct sockaddr_in usersInf;
};

int CreateServerSocket (int socketfd, struct sockaddr_in *serverAddress);
int GetUserNumber      (struct sockaddr_in *clientAddress,
                        struct User** usersList, int  numUsers);
int CreateNewUser      (struct sockaddr_in *clientAddress,
                        struct User** usersList, int *numUsers, char *userName);

char *MakeAnswer (char* nick, char* msg);

int main()
{
	int socketfd, clilen, nUser, numUsers = 0;
	char* line = (char*) calloc (MAX_MSG_LEN, sizeof (char));

	struct User* usersList[MAX_USER_NUM];
	struct sockaddr_in serverAddress, clientAddress;

	if ((socketfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}
	else
		printf("Sock fd:%d\n", socketfd);

	CreateServerSocket (socketfd, &serverAddress);

	while (1)
	{
		bzero (line, MAX_MSG_LEN);

		clilen = sizeof (clientAddress);
		if (recvfrom (socketfd, line, 999, 0,
			(struct sockaddr*) &clientAddress, (socklen_t*) &clilen) < 0)
		{
			perror (NULL);
			close (socketfd);
			exit (1);
		}

		if ((nUser = GetUserNumber (&clientAddress, usersList, numUsers)) == -1)
		{
			CreateNewUser (&clientAddress, usersList, &numUsers, line);

			printf("New user :%s\n", line);

			char Admin[] = "Admin";
			line = strcat(line," join.\n" );
			line = MakeAnswer ((char*) Admin, line);
		}
		else
		{
			line = MakeAnswer (usersList[nUser] -> nick, line);
		}

		for (int i = 0; i < numUsers; i++)
		{
			if (sendto(socketfd, line, strlen(line), 0,
				(struct sockaddr*) &(usersList[i] -> usersInf) , clilen) < 0)
			{
				perror(NULL);
				close(socketfd);
				exit(1);
			}
		}
	}

	free (line);
	return 0;
}

int CreateServerSocket (int socketfd, struct sockaddr_in *serverAddress)
{
	bzero(serverAddress, sizeof (struct sockaddr_in));

	serverAddress -> sin_family      = AF_INET;
	serverAddress -> sin_port        = htons(53000);
	serverAddress -> sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind (socketfd, (struct sockaddr*) serverAddress,
		sizeof (struct sockaddr_in)) < 0)
	{
		perror (NULL);
		close (socketfd);
		exit (1);
	}
}

int GetUserNumber      (struct sockaddr_in *clientAddress,
                        struct User** usersList, int numUsers)
{
	int nUser = -1;

	for (int i = 0; i < numUsers; i++)
	{
		if (((usersList[i] -> usersInf.sin_addr.s_addr) == (clientAddress -> sin_addr.s_addr))&&
			((usersList[i] -> usersInf.sin_port)        == (clientAddress -> sin_port       )))
		{
			nUser = i;
			break;
		}
	}

	return nUser;
}

int CreateNewUser      (struct sockaddr_in *clientAddress,
                        struct User** usersList, int *numUsers, char *userName)
{
	usersList[*numUsers] = (struct User*) calloc (1, sizeof (struct User));

	usersList[*numUsers] -> nick     = (char*) calloc (strlen (userName) + 1, sizeof(char));
	usersList[*numUsers] -> nick     = strcpy (usersList[*numUsers] -> nick, userName);
	usersList[*numUsers] -> usersInf = *clientAddress;

	(*numUsers)++;
}

char *MakeAnswer(char* nick, char* msg)
{
	char* tmp     = (char*) calloc (MAX_MSG_LEN, sizeof (char));
	char* timeStr = (char*) calloc (35         , sizeof (char));

	const time_t timer = time (NULL);
	timeStr            = strcpy (timeStr, ctime (&timer));

	tmp = strcpy(tmp, nick);
	tmp = strcat(tmp, (char*) ": ");
	tmp = strcat(tmp, msg);
	tmp = strcat(tmp, timeStr);
	msg = strcpy(msg, tmp);

	free (tmp);
	free (timeStr);

	return(msg);
}
