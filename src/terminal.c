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
	free(line);
	par = init_paser(lexer);
	root_tree = parser_function(par, 0);
	
	to_free = to_free_struct();
	to_free->par = par;
	to_free->root_tree = root_tree;
	to_free->lexer = lexer;
	global->exit_code = executor_function(root_tree);
	/* TODO: Change it to the free_to_free_structure */
	free_lexer(lexer);
	free_ast(root_tree);
	free_parser_struct(par);
	return (EXIT_SUCCESS);
}

void	terminal()
{
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	pl->prompt = ft_strdup("\033[35m$minishell> \033[0m");
	while (1)
	{
		pl->line = readline(pl->prompt);
		if (ft_strlen(pl->line) == 4 && ft_strncmp(pl->line, "exit", 4) == 0)
		{
			rl_clear_history();
			printf("goes through this exit\n");
			free_global_struct();
			exit_builtin();
		}
		else if (ft_strlen(pl->line) <= 0 && check_only_terminal(pl->line))
		{
			free(pl->line);
			continue;
		}
		else
			add_history(pl->line);
		run_command(pl->line);
	}
}