#include <signal.h>
#include <stdio.h>
#include <strings.h>

void	sigin_handler (int signal)
{
	if (signal == SIGINT)
	{
		printf("\nInterrupted a SIGINT!\n");
		printf("We can start a new program instead of follwoing the dafualt behavior\n");
	}
}

void	set_signal_actions(void)
{
	struct sigaction act;

	// bzero(&act, sizeof(act));
	act.sa_handler = &sigin_handler;
	sigaction(SIGINT, &act, NULL);
}

// int	main(void)
// {
// 	set_signal_actions();

// 	while (1)
// 		continue;
// 	return (0);
// }