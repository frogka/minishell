#include "../includes/minishell.h"

void	sigint_handler(int signal)
{
	t_global	*global;

	global = global_struct();
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	global->exit_code = 130;
}

void	print_child_signals(int status)
{
	if (status == SIGINT)
		write(1, "\n", 1);
	else if (status == SIGQUIT)
		write(1, "Quit: \n", 7);
}

void	parent_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
