#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

int main ()
{
	int N = 0, i = 0;
	int status = 0;
	int pid    = 0, id = 0, 
	    mainid = getpid ();
	    
	scanf ("%d", &N);
	
	printf ("I am a main process\n");
	printf ("My name is   %d\n\n" , getpid ());

	while (pid == 0 && i < N)
	{
		pid = fork ();
		
		if (pid == 0)
		{
			sleep (i);
			printf ("Creating process number %d\n", i);
			printf ("Id %d, parent id %d\n\n", getpid (), getppid ());
		}
		i++;
	}
	
	/*
		Some code
	*/
	
	if (pid == 0)
	{
		printf ("I am the last process\n");
		printf ("My id is %d\n\n", getpid ());
		exit (0);
	}
	
	id = wait (&status);
	
	if (mainid != getpid ()) 
	{
		printf ("My child was %d\n", id);
		printf ("My name is   %d and I am diyng\n\n", getpid ());
		exit (0);
	}
	
	printf ("I am a main process\n");
	printf ("My child was %d\n", id);
	printf ("My name is   %d\n" , getpid ());
		
	return 0;
}
