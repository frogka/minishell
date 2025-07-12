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

int	lexer_function(char *input, t_lexer *lexer)
{
	int		i;
	int		j;
	t_token *curr_token;
	int	len_input;
	
	i = 0;
	j = 0;
	len_input = ft_strlen(input);
	// need to create first Token for malloc if there are no major errors is correct
	if(check_matching_quotes(input))
		return (1);
	if (!lexer)
		return (1);
	lexer->first_token = init_token(lexer->first_token, len_input);
	curr_token = lexer->first_token;
	lexer->count_token++;
	printf("%p\n",curr_token);
	while (input[i])
	{
		printf("[%i]: %s\n", i, curr_token->content);
		if (input[i] == '&' && input[i + 1] == '&')
		{
			curr_token->content[j] = 0;
			j = 0;
			curr_token = add_token_back(lexer, len_input);
			curr_token->content[j] = input[i++];
			curr_token->content[j + 1] = input[i];
			curr_token->type = CHAR_AND;
		}
		else if (input[i] == '>' && input[i + 1] == '>')
		{
			curr_token->content[j] = 0;
			j = 0;
			curr_token = add_token_back(lexer, len_input);
			curr_token->content = ft_substr(input, i, 2);
			curr_token->type = CHAR_APPEND;
			i++;
		}
		else if (input[i] == '<' && input[i + 1] == '<')
		{
			curr_token->content[j] = 0;
			j = 0;
			curr_token->next = add_token_back(lexer, len_input);
			curr_token = curr_token->next;
			curr_token->content = ft_substr(input, i, 2);
			curr_token->type = CHAR_HEREDOC;
			i++;
		}
		else if (input[i] == '|' && input[i + 1] == '|')
		{
			curr_token->content[j] = 0;
			j = 0;
			curr_token = add_token_back(lexer, len_input);
			curr_token->content[j] = input[i++];
			curr_token->content[j + 1] = input[i];
			curr_token->type = CHAR_OR;
			curr_token = add_token_back(lexer, len_input);
		}
		else if (input[i] == CHAR_PIPE || input[i] == CHAR_OUTRED || input[i] == CHAR_INRED || input[i] == CHAR_AMPERSAND || input[i] == CHAR_QM || input[i] == CHAR_DOLLAR)
		{
			curr_token->content[j] = 0;
			j = 0;
			curr_token = add_token_back(lexer, len_input);
			curr_token->content[j] = input[i];
			curr_token->type = input[i];
			curr_token = add_token_back(lexer, len_input);
		}
		else if (input[i] == CHAR_SPACE || input[i] == CHAR_NEWLINE || input[i] == CHAR_TAB)
		{
			input[i];
		}
		else
		{
			curr_token->content[j] = input[i];
			curr_token->type = CHAR_DEF;
			j++;
		}
		i++;
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