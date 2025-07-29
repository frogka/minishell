#include "../includes/minishell.h"

t_global *global_struct(void)
{
	static t_global	global;

	return (&global);
}

t_to_free	*to_free_struct(void)
{
	static t_to_free	to_free;

	return (&to_free);
}

void	init_ev(char *envp[])
{
	t_global	*global;
	int			count;

	global = global_struct();
	count = 0;
	if(envp == NULL || envp[0] == NULL)
	{
		global->ev = NULL;
		return ;
	}
	while (envp[count])
		count++;
	global->ev = malloc(sizeof(char*) * (count + 1));
	count = -1;
	while (envp[++count])
		global->ev[count] = ft_strdup(envp[count]);
	global->ev[count] = NULL;
}

void	init_global_struct(char *envp[])
{
	t_global *global;

	global = global_struct();
	global->exit_code = 0;
	init_ev(envp);
}

void	free_global_struct(void)
{
	t_global	*global;
	int			i;

	global = global_struct();
	if (global->ev == NULL)
		return;	
	i = -1;
	while (global->ev[++i])
		free(global->ev[i]);
	free(global->ev);
}

void	free_lexer(t_lexer *lexer)
{
	t_token	*temp;

	while (lexer->first_token)
	{
		temp = lexer->first_token;
		lexer->first_token = lexer->first_token->next;
		free(temp->data);
		free(temp);
	}
	free(lexer);
}

t_lexer	*init_lexer(void)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
	{
		exit(EXIT_FAILURE);
	}
	lexer->first_token = NULL;
	lexer->count_token = 0;
	return (lexer);
}