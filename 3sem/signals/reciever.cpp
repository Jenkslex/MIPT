#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int number   = 0;
int counter  = 8 * sizeof (int);
int senderid = 0;

void Operation (int nsig)
{
	int message = (nsig == SIGUSR1) ? 0 : 1;

	number = number | (message << (8 * sizeof (int) - counter));

	counter--;

	printf ("number  = %d\n", number);
	printf ("counter = %d\n", counter);
	
	if (counter != 0) kill (senderid, SIGUSR1);
}


int main(void)
{
	signal (SIGUSR1, Operation);
	signal (SIGUSR2, Operation);

	printf ("My pid = %d\n", getpid ());
	
	printf ("Enter pid of sender: ");
	scanf  ("%d", &senderid);

	while (counter != 0);

	printf ("Number: %d\n", number);

	return 0;
}
