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
	// configure les signaux avant l'appel a terminal
	struct sigaction sa;
	
	sa.sa_handler = handler;      // On associe notre fonction au signal
	sigemptyset(&sa.sa_mask);     // On ne bloque aucun autre signal pendant le handler
	sa.sa_flags = SA_RESTART;              // Pas d'option spéciale
	
	sigaction(SIGINT, &sa, NULL); // On applique ça au signal SIGINT
    signal(SIGQUIT, SIG_IGN);
	                
	terminal();
	return (0);
}
