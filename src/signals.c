#include "../includes/minishell.h"
#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>


void	handler(int sig)
{
	if (sig == SIGINT || sig == SIGQUIT)
	{
		g_sig_received = sig;
		if(sig == SIGINT && global_struct()->in_readline)
		{
			write(1, "\n", 1);
			rl_on_new_line();              // readline sait qu'on est sur une nouvelle ligne
			rl_replace_line("", 0);       // remplace la ligne courante par rien
			rl_redisplay();               // réaffiche le prompt proprement
			global_struct()->exit_code = 130;
		}
	}	
	//else if(sig == SIGQUIT)
	//	g_sig_received = SIGQUIT;

    //else if (sig == SIGQUIT)
	//	return;
}
/*
void handle_sigint(int sig)
{
	if (g_sig_received == SIGINT)
	{
		write(1, "\n", 1);
		
	}
}*/
/*
int	main(void)
{
	struct sigaction sa;

	sa.sa_handler = handler;      // On associe notre fonction au signal
	sigemptyset(&sa.sa_mask);     // On ne bloque aucun autre signal pendant le handler
	sa.sa_flags = 0;              // Pas d'option spéciale

	sigaction(SIGINT, &sa, NULL); // On applique ça au signal SIGINT
    sigaction(SIGQUIT, &sa, NULL);

	while (1)                     // Programme infini pour tester
		pause();                 // Attend un signal
}
*/