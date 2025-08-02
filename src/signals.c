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

void	sigint_test(int signal)
{
	(void)signal;
	write(1, "IN HEREREIJRPEIJREJR\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	write(1, "In Here\n", 8);
}

void	parent_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	child_signals(void)
{
	signal(SIGINT, sigint_test);
	signal(SIGQUIT, SIG_DFL);
}
