#include "../includes/minishell.h"

int	check_matching_quotes(char *input)
{
	int		i;
	int		counter;
	char	quote_type;

	i = 0;
	counter = 0;
	while (input[i])
	{
		if (input[i] == '\\' && (input[i + 1] == '\'' || input[i + 1] == '\"'))
		{
			i += 2;
			continue;
		}
		if (counter == 0 && (input[i] == '\"' || input[i] == '\''))
		{
			counter++;
			quote_type = input[i];
		}
		else if (counter == 1 && input[i] == quote_type)
			counter = 0;
		i++;		
	}
	if (counter > 0)
	{
		printf("we have a problem!\n");
		return (1);		
	}
	return (0);
}

t_token	*get_last_token(t_lexer *lexer)
{
	t_token	*last_token;

	if (!lexer)
		return (NULL);
	if (lexer->first_token == NULL)
		return (lexer->first_token);
	last_token = lexer->first_token;
	while (last_token->next != NULL)
		last_token = last_token->next;
	return (last_token);
}

t_token *init_token(t_token *token, int len_input)
{
	token = malloc(sizeof(t_token));
	token->content = ft_calloc(len_input, sizeof(char));
	token->next = NULL;
	token->type = 0;
	return(token);
}

t_token	*add_token_back(t_lexer *lexer, int len_input)
{
	t_token	*last_token;
	t_token	*temp_token;

	if (lexer->first_token == NULL)
	{
		lexer->first_token = init_token(lexer->first_token, len_input);
		lexer->count_token++;
		return (lexer->first_token);
	}
	else
	{
		temp_token = get_last_token(lexer);
		last_token = init_token(lexer->first_token, len_input);
		temp_token->next = last_token;
		lexer->count_token++;
		return (last_token);
	}
}

void	handle_def_1char(char *input, t_token_aux *aux, t_lexer *lexer, int *f)
{
	if (input[aux->i] == CHAR_PIPE || input[aux->i] == CHAR_OUTRED ||
		input[aux->i] == CHAR_INRED || input[aux->i] == CHAR_AMPERSAND ||
		input[aux->i] == CHAR_QM || input[aux->i] == CHAR_DOLLAR)
	{
		aux->curr_token->content[aux->j] = 0;
		aux->j = 0;
		aux->curr_token = add_token_back(lexer, aux->len_input);
		aux->curr_token->content[aux->j] = input[aux->i];
		aux->curr_token->type = input[aux->i];
		aux->curr_token = add_token_back(lexer, aux->len_input);
		(*f)++;
	}
}

void	handle_def_2char(char *input, t_token_aux *aux, t_lexer *lexer, int *f)
{
	if (input[aux->i] == '>' && input[aux->i + 1] == '>' && (*f)++)
		aux->curr_token->type = CHAR_APPEND;
	else if (input[aux->i] == '<' && input[aux->i + 1] == '<' && (*f)++)
		aux->curr_token->type = CHAR_HEREDOC;
	else if (input[aux->i] == '&' && input[aux->i + 1] == '&' && (*f)++)
		aux->curr_token->type = CHAR_AND;
	else if (input[aux->i] == '|' && input[aux->i + 1] == '|' && (*f)++)
		aux->curr_token->type = CHAR_OR;
	if ((*f) == 1)
	{
		aux->curr_token->content[aux->j] = 0;
		aux->j = 0;
		aux->curr_token = add_token_back(lexer, aux->len_input);
		aux->curr_token->content[aux->j] = input[(aux->i)++];
		aux->curr_token->content[aux->j + 1] = input[aux->i];		
	}
}

void	process_char(char *input, t_token_aux *aux, t_lexer *lexer)
{
	int	flag;

	flag = 0;
	handle_def_2char(input, aux, lexer, &flag);
	handle_def_1char(input, aux, lexer, &flag);
	if (flag == 0)
	{
		aux->curr_token->content[aux->j] = input[aux->i];
		aux->curr_token->type = CHAR_DEF;
		aux->j++;
	}	
}

int	lexer_function(char *input, t_lexer *lexer)
{
	t_token_aux	aux;
	// int		status;
	
	aux.i = 0;
	aux.j = 0;
	aux.len_input = ft_strlen(input);
	if(check_matching_quotes(input))
		return (1);
	if (!lexer)
		return (1);
	lexer->first_token = init_token(lexer->first_token, aux.len_input);
	aux.curr_token = lexer->first_token;
	lexer->count_token++;
	printf("%p\n",aux.curr_token);
	while (input[aux.i])
	{		
		printf("[%i]: %s\n", aux.i, aux.curr_token->content);
		process_char(input, &aux, lexer);
		// else if (input[i] == CHAR_SPACE || input[i] == CHAR_NEWLINE || input[i] == CHAR_TAB)
		// {
		// 	input[i];
		// }
		aux.i++;
	}
	return (0);
}

int main()
{
	t_lexer	*lexer;
	char *test = "ls -l || grep \"this file\\\"\" ?user   $VAR";

	printf("%s\n", test);
	lexer = malloc(sizeof(t_lexer));
	lexer->first_token = NULL;
	lexer->count_token = 0;
	if (!lexer)
		return (EXIT_FAILURE);
	lexer_function(test, lexer);
	while (lexer->first_token)
	{
		printf("This is the content: %s\n", lexer->first_token->content);
		lexer->first_token = lexer->first_token->next;
	}
	
	return (0);
}