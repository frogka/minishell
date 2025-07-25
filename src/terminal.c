#include "../includes/minishell.h"

int	run_command(char *line)
{
	t_parser	*par;
	t_ast		*root_tree;
	t_lexer		*lexer;
	t_global	*global;

	/* inits */
	global = global_struct();
	lexer = init_lexer();
	if (!lexer)
		return (EXIT_FAILURE);

	lexer_function(line, lexer);

	par = init_paser(lexer);
	root_tree = parser_function(par, 0);
	// print_ast_node(root_tree);
	// print_ast_sexpr(root_tree);

	global->exit_code = executor_function(root_tree);
	free_lexer(lexer);
	free_ast(root_tree);
	free_parser_struct(par);
	return (EXIT_SUCCESS);
}

void	terminal()
{
	// t_global	*global;
	char		*prompt;
	char		*line;

	// global = global_struct();
	prompt = ft_strdup("\033[35m$minishell> \033[0m");
	while (1)
	{
		line = readline(prompt);
		if (ft_strlen(line) <= 0 && check_only_terminal(line))
		{
			free(line);
			continue;
		}
		else
			add_history(line);
		run_command(line);
	}
}