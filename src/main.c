#include "../includes/minishell.h"

int main(int argc, char *argv[], char *envp[])
{
	t_parser	*par;
	t_ast		*root_tree;
	t_lexer		*lexer;
	char *test1 = argv[1];
	char *test = "grep \"$USER - THIS IS\n\n $PATH o yeah $? $\"";
	// char *test = "cat | grep";
	// char *test = "cat | grep | ls";
	// char *test = "ls -l cat cat | grep this | ls";
	// char *test = "<input.txt ls -l cat cat | grep this | ls";
	// char *test = "<input.txt ls -l cat cat >ouput1 | grep this >ouput2 | ls >output3";
	// char *test = "|";

	if (argc == -1)
		printf("Don't forget to only provide one string\n");
	if (test1 == NULL)
	{
		;
	}
	printf("This is the string being tested: '%s'\n", test); 
	
	/* inits */
	init_global_struct(envp);
	lexer = init_lexer();
	if (!lexer)
		return (EXIT_FAILURE);

	lexer_function(test, lexer);

	par = init_paser(lexer);
	root_tree = parser_function(par, 0);
	print_ast_node(root_tree);
	print_ast_sexpr(root_tree);

	executor_function(root_tree);

	free_lexer(lexer);
	free_global_struct();
	free_ast(root_tree);
	free_parser_struct(par);
	return (0);
}
