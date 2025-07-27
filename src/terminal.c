#include "../includes/minishell.h"

int	run_command(char *line)
{
	t_parser	*par;
	t_ast		*root_tree;
	t_lexer		*lexer;
	t_global	*global;
	t_to_free	*to_free;

	/* inits */
	global = global_struct();
	lexer = init_lexer();
	if (!lexer)
		return (EXIT_FAILURE);

	lexer_function(line, lexer);

	par = init_paser(lexer);
	root_tree = parser_function(par, 0);
	
	to_free = to_free_struct();
	to_free->par = par;
	to_free->root_tree = root_tree;
	to_free->lexer = lexer;
	global->exit_code = executor_function(root_tree);
	free_lexer(lexer);
	free_ast(root_tree);
	free_parser_struct(par);
	return (EXIT_SUCCESS);
}

void	terminal()
{
	char		*prompt;
	char		*line;

	prompt = ft_strdup("\033[35m$minishell> \033[0m");
	while (1)
	{
		line = readline(prompt);
		if (ft_strlen(line) == 4 && ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			free(prompt);
			rl_clear_history();
			printf("goes through this exit\n");
			free_global_struct();
			break;
		}
		else if (ft_strlen(line) <= 0 && check_only_terminal(line))
		{
			free(line);
			continue;
		}
		else
			add_history(line);
		run_command(line);
	}
}