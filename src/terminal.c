#include "../includes/minishell.h"

/* TODO: terminal is wrapping aroung and going over the prompt*/

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

	if (lexer_function(line, lexer) == EXIT_FAILURE)
	{
		free_lexer(lexer);
		return (EXIT_FAILURE);
	}
	free(line);
	par = init_paser(lexer);
	// for(int i = 0; i < lexer->count_token; i++)
	// {
	// 	printf("[%i] %s\n", lexer->first_token->type, lexer->first_token->data);
	// 	lexer->first_token = lexer->first_token->next;
	// }
	
	root_tree = parser_function(par, 0);
	print_ast_sexpr(root_tree);
	
	to_free = to_free_struct();
	to_free->par = par;
	to_free->root_tree = root_tree;
	to_free->lexer = lexer;
	global->exit_code = executor_function(root_tree);
	free_struct_to_free();
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
		if (pl->line == NULL)
			continue;
		if (ft_strlen(pl->line) == 4 && ft_strncmp(pl->line, "exit", 4) == 0)
		{
			rl_clear_history();
			free_global_struct();
			exit_builtin();
		}
		else if (ft_strlen(pl->line) > 0 && check_only_terminal(pl->line))
		{
			free(pl->line);
			continue;
		}
		else
			add_history(pl->line);
		run_command(pl->line);
	}
}