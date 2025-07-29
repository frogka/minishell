#include "../includes/minishell.h"

int main(int argc, char *argv[], char *envp[])
{

	// char *test1 = argv[1];
	// char *test = "grep \"$USER - THIS IS\n\n $PATH o yeah $? $\"";
	// char *test = "cat | grep";
	// char *test = "cat | grep | ls";
	// char *test = "ls -la | grep minishell";
	// char *test = "ls -la";
	// char *test = "ls -l cat cat | grep this | ls";
	// char *test = "<input.txt ls -l cat cat | grep this | ls";
	// char *test = "<input.txt ls -l cat cat >ouput1 | grep this >ouput2 | ls >output3";
	// char *test = "<bnf.txt cat > output2";
	// char *test = "<bnf.txt cat | grep commandline >ouput2";
	// char *test = "<bnf.txt cat | grep commandline";
	// char *test = "|";
	// char *test = "<bnf.txt";
	// char *test = ">output";
	// char *test = "echo $USER";
	// char *test = "echo$USER";
	// char *test = "echo$USE";
	// char *test = "echo $PATH";
	// char *test = "<<DATA cat | grep This";
	// char *test = "<<DATA cat | grep This | <<NOTHING cat";
	// char *test = "<<DATA cat";

	(void)argv;
	if (argc != 1)
	{
		printf("minishell: Cannot be called with any parameter\n");
		return (EXIT_SUCCESS);
	}
	init_global_struct(envp);
	// configure les signaux avant l'appel a terminal
	struct sigaction sa;
	
	sa.sa_handler = handler;      // On associe notre fonction au signal
	sigemptyset(&sa.sa_mask);     // On ne bloque aucun autre signal pendant le handler
	sa.sa_flags = 0;              // Pas d'option spéciale
	
	sigaction(SIGINT, &sa, NULL); // On applique ça au signal SIGINT
    sigaction(SIGQUIT, &sa, NULL);
	                
	terminal();
	return (0);
}
