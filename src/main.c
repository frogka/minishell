#include "../includes/minishell.h"

int main(int argc, char *argv[], char *envp[])
{
	(void)argv;
	if (argc != 1)
	{
		printf("minishell: Cannot be called with any parameter\n");
		return (EXIT_SUCCESS);
	}
	init_global_struct(envp);
	terminal();
	return (0);
}
