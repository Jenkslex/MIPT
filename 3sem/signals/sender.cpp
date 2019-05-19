#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/*
 * 1) Вы считываете 4х байтное число, а отсылаете из него только один.
 * 2) Скорость передачи - 1 bit per sec. Неплохо было бы организовать синхронизацию отправителя и получателя,
 * задействовав ещё один сигнал. Этот сигнал получатель высылал отправителю, мол шли следующий бит.
 * 
 * Не очень здорово, но править уже некогда.
 */

int number  = 0;
int pid     = 0;
int counter = 8 * sizeof (int);

void Operation (int nsig)
{
	printf ("number = %d\n", number);

	if ((number % 2) == 0) kill (pid, SIGUSR1);
	else                   kill (pid, SIGUSR2);
	
	number = number / 2;

	counter--;
}

int main(void)
{
	signal (SIGUSR1, Operation);

	printf ("Enter a number to send: ");
	scanf  ("%d", &number);

	printf ("Pid of sender is %d\n", getpid ());

	printf ("Enter the pid of reciever: ");
	scanf  ("%d", &pid);
	
	Operation (0);
	
	while (counter != 0);

	return 0;
}
