#include "../includes/minishell.h"

/* Start Init functions */

void	init_lexer_aux(char *input, t_token_aux *aux, t_lexer *lexer)
{
	aux->i = 0;
	aux->j = 0;
	aux->len_input = ft_strlen(input);
	lexer->first_token = init_token(lexer->first_token, aux->len_input);
	lexer->count_token++;
	aux->curr_token = lexer->first_token;
	aux->status = CHAR_DEF;
}

t_token	*init_token(t_token *token, int len_input)
{
	token = malloc(sizeof(t_token));
	token->data = ft_calloc(len_input + 1, sizeof(char));
	token->next = NULL;
	token->type = 0;
	return (token);
}

/* End Init functions */

/* Start Aux functions */

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
		ft_putstr_fd("Error: Unclosed quotation detected\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_matching_parenthesis(t_lexer *lexer)
{
	int		i;
	int		counter;
	t_token	*curr_token;

	i = -1;
	counter = 0;
	curr_token = lexer->first_token;
	while (++i < lexer->count_token)
	{
		if (curr_token->type == CHAR_CPAREN)
			counter--;
		else if (curr_token->type == CHAR_OPAREN)
			counter++;
		if (counter < 0)
		{
			ft_putstr_fd("Error: Invalid Parenthesis\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		curr_token = curr_token->next;
	}
	if (counter != 0)
	{
		ft_putstr_fd("Error: Invalid Parenthesis\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
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

/* End Aux functions */

/* Start of lexer functions */

int	lexer_function(char *input, t_lexer *lexer)
{
	t_token_aux	aux;

	if (check_only_terminal(input) == 1)
	{
		lexer->first_token = NULL;
		lexer->count_token = 0;
		return (EXIT_SUCCESS);
	}
	if (check_matching_quotes(input) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!lexer)
		return (EXIT_FAILURE);
	init_lexer_aux(input, &aux, lexer);
	process_char(input, &aux, lexer);
	clean_last_tokens(&aux, lexer);
	token_expansion(&aux, lexer);
	if (check_matching_parenthesis(lexer) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	handle_def_1char(char *input, t_token_aux *aux, t_lexer *lexer, int *f)
{
	if (*f == 0 && (input[aux->i] == CHAR_PIPE || input[aux->i] == CHAR_OUTRED
			|| input[aux->i] == CHAR_INRED || input[aux->i] == CHAR_AMPERSAND
			|| input[aux->i] == CHAR_OPAREN || input[aux->i] == CHAR_CPAREN))
	{
		if (aux->j != 0)
		{
			aux->curr_token->data[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(lexer, aux->len_input);
		}
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->curr_token->data[aux->j + 1] = 0;
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
			aux->curr_token->data[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(lexer, aux->len_input);
		}
		aux->curr_token->data[aux->j] = input[(aux->i)++];
		aux->curr_token->data[aux->j + 1] = input[aux->i];
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
			aux->curr_token->data[aux->j] = 0;
			aux->j = 0;
			aux->curr_token = add_token_back(lexer, aux->len_input);
		}
		(*f)++;
	}
}

void	handle_start_quote(char *input, t_token_aux *aux, int *f)
{
	if (*f == 0 && input[aux->i] == CHAR_DQUOTE)
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->status = input[aux->i];
		aux->j++;
		(*f)++;
	}
	else if (*f == 0 && input[aux->i] == CHAR_QUOTE)
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->status = input[aux->i];
		aux->j++;
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
	handle_start_quote(input, aux, &flag);
	if (flag == 0)
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->curr_token->type = CHAR_DEF;
		aux->j++;
	}
}

void	process_char_quote(char *input, t_token_aux *aux)
{
	if (input[aux->i] == '\\' && input[aux->i + 1] == aux->status)
	{
		aux->curr_token->data[(aux->j)++] = input[aux->i];
		aux->curr_token->data[(aux->j)++] = input[aux->i + 1];
		aux->i += 2;
	}
	if (input[aux->i] == aux->status)
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->j++;
		aux->curr_token->type = CHAR_DEF;
		aux->status = CHAR_DEF;
	}
	else
	{
		aux->curr_token->data[aux->j] = input[aux->i];
		aux->curr_token->type = aux->status;
		aux->j++;
	}
}

void	process_char(char *input, t_token_aux *aux, t_lexer *lexer)
{
	while (input[aux->i])
	{
		if (aux->status == CHAR_DEF)
			process_char_def(input, aux, lexer);
		else
			process_char_quote(input, aux);
		aux->i++;
	}
}

void	clean_last_tokens(t_token_aux *aux, t_lexer *lexer)
{
	t_token		*temp;

	if (aux->curr_token->data[0] == 0 && aux->curr_token->type == 0)
	{
		temp = aux->curr_token;
		free(temp->data);
		free(temp);
		aux->curr_token = get_previous_token(lexer->first_token, aux->curr_token);
		aux->curr_token->next = NULL;
		lexer->count_token--;
	}
}

/* End of lexer functions */

/* Start expansion functions */

void	insert_expansion(t_token *token, int sta, int len, char *mid_str)
{
	char	*start_str;
	char	*end_str;
	char	*final_str;
	int		len_str;
	int		len_final;
	int		i;
	int		j;

	len_str = ft_strlen(token->data);
	start_str = ft_substr(token->data, 0, sta);
	end_str = ft_substr(token->data, sta + len, len_str - (sta + len));

	len_final = 0;
	len_final += ft_strlen(start_str);
	len_final += ft_strlen(mid_str);
	len_final += ft_strlen(end_str);
	final_str = malloc(sizeof(char) * (len_final + 1));

	i = -1;
	while (start_str[++i])
		final_str[i] = start_str[i];
	j = i;
	i = -1;
	while (mid_str[++i])
		final_str[i + j] = mid_str[i];
	j += i;
	i = -1;
	while (end_str[++i])
		final_str[i + j] = end_str[i];
	final_str[len_final] = 0;
	free(start_str);
	free(mid_str);
	free(end_str);
	free(token->data);
	token->data = final_str;
}

int	token_quote_removal(t_token *token, int *i, int *status, int quote_type)
{
	if (token->data[*i] == quote_type && *status == CHAR_DEF)
	{
		*status = quote_type;
		ft_rmvchr(token->data, &token->data[*i]);
		return (1);
	}
	else if (token->data[*i] == '\\' && token->data[*i + 1] == quote_type
				&& *status == quote_type)
	{
		ft_rmvchr(token->data, &token->data[*i]);
		(*i)++;
		return (1);
	}
	else if (token->data[*i] == quote_type && *status == quote_type)
	{
		*status = CHAR_DEF;
		ft_rmvchr(token->data, &token->data[*i]);
		return (1);
	}
	return (0);
}

/* TODO: 
===> create a new char* to hold the new version of the token after the expansion
===> hold a status in that keeps track if you entered into a part between ''
*/
void	token_expansion_aux(t_token *token)
{
	char		*temp;
	char		*to_expand;
	int			i;
	int			j;
	int			len;
	t_global	*global;
	int			status;

	i = 0;
	j = 0;
	global = global_struct();
	status = CHAR_DEF;
	while (token->data[i])
	{
		if(token_quote_removal(token, &i, &status, CHAR_QUOTE))
			continue;
		if(token_quote_removal(token, &i, &status, CHAR_DQUOTE))
			continue;
		j = 0;
		if (token->data[i] == CHAR_DOLLAR 
				&& token->data[i + 1] != 0
				&& token->data[i + 1] == '?'
				&& status != CHAR_QUOTE)
		{
			temp = ft_itoa(global->exit_code);
			len = ft_strlen(temp);
			insert_expansion(token, i, len + 1, temp);
		}
		else if (token->data[i] == CHAR_DOLLAR 
				&& token->data[i + 1] != 0
				&& token->data[i + 1] != ' '
				&& token->data[i + 1] != CHAR_DQUOTE
				&& status != CHAR_QUOTE)
		{
			while (token->data[i + 1 + j] != 0
					&& token->data[i + 1 + j] != ' '
					&& token->data[i + 1 + j] != '\"'
					&& token->data[i + 1 + j] != '\''
					&& token->data[i + 1 + j] != '\\')
			{
				j++;
			}
			temp = ft_substr(token->data, i + 1, j);
			to_expand = find_ev(temp);
			free(temp);
			insert_expansion(token, i, j + 1, to_expand);
		}
		i++;
	}
}

void	token_expansion(t_token_aux *aux, t_lexer *lexer)
{
	if (lexer == NULL || lexer->first_token == NULL)
		return;
	aux->curr_token = lexer->first_token;
	while (aux->curr_token)
	{
		if (aux->curr_token->type == CHAR_DQUOTE
				|| aux->curr_token->type == CHAR_DEF)
			token_expansion_aux(aux->curr_token);
		aux->curr_token = aux->curr_token->next;
	}
}

/* End expansion functions */