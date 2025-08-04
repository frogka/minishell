#include "../includes/minishell.h"

int g_sig_received = 0;

int main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc != 1)
	{
		printf("minishell: Cannot be called with arguments\n");
		return (EXIT_SUCCESS);
	}
	init_global_struct(envp);
	init_signals();
	terminal();
	return (0);
}
