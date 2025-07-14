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
			continue ;
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

t_token	*get_previous_token(t_token *first_token, t_token *curr_token)
{
	t_token	*previous_token;

	if (!first_token)
		return (NULL);
	previous_token = first_token;
	while (previous_token->next != curr_token && previous_token->next != NULL)
		previous_token = previous_token->next;
	return (previous_token);
}

t_token	*init_token(t_token *token, int len_input)
{
	token = malloc(sizeof(t_token));
	token->content = ft_calloc(len_input, sizeof(char));
	token->next = NULL;
	token->type = 0;
	return (token);
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
	if (*f == 0 && (input[aux->i] == CHAR_PIPE || input[aux->i] == CHAR_OUTRED
			|| input[aux->i] == CHAR_INRED || input[aux->i] == CHAR_AMPERSAND
			|| input[aux->i] == CHAR_QM || input[aux->i] == CHAR_DOLLAR))
	{
		if (aux->j != 0)
		{
			aux->curr_token->content[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(lexer, aux->len_input);
		}
		aux->curr_token->content[aux->j] = input[aux->i];
		aux->curr_token->type = input[aux->i];
		aux->curr_token = add_token_back(lexer, aux->len_input);
		(*f)++;
	}
}

void	handle_def_2char(char *input, t_token_aux *aux, t_lexer *lexer, int *f)
{
	if (input[aux->i] == '>' && input[aux->i + 1] == '>')
	{
		aux->curr_token->type = CHAR_APPEND;
		(*f)++;
	}
	else if (input[aux->i] == '<' && input[aux->i + 1] == '<')
	{
		aux->curr_token->type = CHAR_HEREDOC;
		(*f)++;
	}
	else if (input[aux->i] == '&' && input[aux->i + 1] == '&')
	{
		aux->curr_token->type = CHAR_AND;
		(*f)++;
	}
	else if (input[aux->i] == '|' && input[aux->i + 1] == '|')
	{
		aux->curr_token->type = CHAR_OR;
		(*f)++;
	}
	if ((*f) == 1)
	{
		if (aux->j != 0)
		{
			aux->curr_token->content[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(lexer, aux->len_input);
		}
		aux->curr_token->content[aux->j] = input[(aux->i)++];
		aux->curr_token->content[aux->j + 1] = input[aux->i];
		aux->curr_token = add_token_back(lexer, aux->len_input);
	}
}

void	handle_terminal(char *input, t_token_aux *aux, t_lexer *lexer, int *f)
{
	if (*f == 0 && (input[aux->i] == CHAR_SPACE
			|| input[aux->i] == CHAR_NEWLINE
			|| input[aux->i] == CHAR_TAB))
	{
		if (aux->j != 0)
		{
			aux->curr_token->content[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(lexer, aux->len_input);
		}
		(*f)++;
	}
}

void	handle_start_quote(char *input, t_token_aux *aux, t_lexer *lexer, int *f)
{
	if (*f == 0 && (input[aux->i] == CHAR_QUOTE
			|| input[aux->i] == CHAR_DQUOTE))
	{
		if (aux->j != 0)
		{
			aux->curr_token->content[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(lexer, aux->len_input);
		}
		aux->status = input[aux->i];
		(*f)++;
	}
}

void	process_char_def(char *input, t_token_aux *aux, t_lexer *lexer)
{
	int	flag;

	flag = 0;
	handle_def_2char(input, aux, lexer, &flag);
	handle_def_1char(input, aux, lexer, &flag);
	handle_terminal(input, aux, lexer, &flag);
	handle_start_quote(input, aux, lexer, &flag);
	if (flag == 0)
	{
		aux->curr_token->content[aux->j] = input[aux->i];
		aux->curr_token->type = CHAR_DEF;
		aux->j++;
	}
}

void	process_char_quote(char *input, t_token_aux *aux, t_lexer *lexer)
{
	if (input[aux->i] == '\\' && input[aux->i + 1] == aux->status)
	{
		aux->curr_token->content[(aux->j)++] = input[aux->i + 1];
		aux->i += 2;
	}
	if (input[aux->i] == aux->status)
	{
		aux->curr_token->content[aux->j] = 0;
		aux->j = 0;
		aux->status = DEF;
		aux->curr_token = add_token_back(lexer, aux->len_input);
	}
	else
	{
		aux->curr_token->content[aux->j] = input[aux->i];
		aux->curr_token->type = aux->status;
		aux->j++;
	}
}

int	check_only_terminal(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n')
			return (0);
		i++;
	}
	if (i == 0 && input[0] == 0)
		return (1);
	return (1);
}

void	init_lexer_aux(char *input, t_token_aux *aux, t_lexer *lexer)
{
	aux->i = 0;
	aux->j = 0;
	aux->len_input = ft_strlen(input);
	lexer->first_token = init_token(lexer->first_token, aux->len_input);
	lexer->count_token++;
	aux->curr_token = lexer->first_token;
	aux->status = DEF;
}

void	process_char(char *input, t_token_aux *aux, t_lexer *lexer)
{
	while (input[aux->i])
	{
		if (aux->status == DEF)
			process_char_def(input, aux, lexer);
		else
		{
			process_char_quote(input, aux, lexer);
		}
		aux->i++;
	}
}

void	clean_last_tokens(t_token_aux *aux, t_lexer *lexer)
{
	t_token		*temp;

	if (aux->curr_token->content[0] == 0 && aux->curr_token->type == 0)
	{
		temp = aux->curr_token;
		free(temp->content);
		free(temp);
		aux->curr_token = get_previous_token(lexer->first_token, aux->curr_token);
		aux->curr_token->next = NULL;
		lexer->count_token--;
	}
}

int	lexer_function(char *input, t_lexer *lexer)
{
	t_token_aux	aux;

	if (check_only_terminal(input) == 1)
	{
		lexer->first_token = NULL;
		lexer->count_token = 0;
		return (0);
	}
	if (check_matching_quotes(input))
		return (1);
	if (!lexer)
		return (1);
	init_lexer_aux(input, &aux, lexer);
	process_char(input, &aux, lexer);
	clean_last_tokens(&aux, lexer);
	return (0);
}

int main(int argc, char *argv[])
{
	t_lexer	*lexer;
	t_token *temp;
	char *test = argv[1];

	if (argc != 2)
		printf("Don't forget to only provide one string\n");
	printf("This is the string being tested: '%s'\n", test);
	lexer = malloc(sizeof(t_lexer));
	lexer->first_token = NULL;
	lexer->count_token = 0;
	if (!lexer)
		return (EXIT_FAILURE);
	lexer_function(test, lexer);
	while (lexer->first_token)
	{
		printf("This is the content: '%s' and this is the type: '%i'\n", lexer->first_token->content, lexer->first_token->type);
		temp = lexer->first_token;
		lexer->first_token = lexer->first_token->next;
		free(temp->content);
		free(temp);
	}
	printf("Number of tokens: %i\n", lexer->count_token);
	free(lexer);
	return (0);
}
