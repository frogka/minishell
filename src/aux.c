#include "../includes/minishell.h"


void	init_ev(char *envp[])
{
	t_global	*global;
	t_list		*temp;
	int			i;

	global = global_struct();
	global->ev = malloc(sizeof(t_list*));
	*global->ev = NULL;
	i = 0;
	while (envp[i])
	{
		temp = malloc(sizeof(t_list));
		temp->content = ft_strdup(envp[i]);
		temp->next = NULL;
		ft_lstadd_back(global->ev, temp);
		i++;
	}
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
	t_list		*temp;

	global = global_struct();
	while ((*global->ev))
	{
		temp = (*global->ev)->next;
		free((*global->ev)->content);
		free((*global->ev));
		(*global->ev) = temp;
	}
	free(global->ev);
}

void	free_lexer(t_lexer *lexer)
{
	t_token	*temp;

	while (lexer->first_token)
	{
		// printf("This is the content: '%s' and this is the type: '%i'\n", lexer->first_token->content, lexer->first_token->type);
		temp = lexer->first_token;
		lexer->first_token = lexer->first_token->next;
		free(temp->content);
		free(temp);
	}
	// printf("Number of tokens: %i\n", lexer->count_token);
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