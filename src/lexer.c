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

int	lexer_function(char *input, t_lexer	*lexer)
{
	// int	i;
	// int	len_input;
	
	// i = 0;
	// len_input = ft_strlen(input);
	// need to create first Token for malloc if there are no major errors is correct
	// need to check if there are matching quotes in the input
	if(check_matching_quotes(input))
		return (1);
	if (!lexer)
		return (1);

	// while (input[i])
	// {
	// 	if (input[i] == '&' && input[i + 1] == '&')
	// 	{
	// 		// to add the content - either by ft_strdup or st_substr
	// 		lexer->first_token->type == CHAR_AND;
	// 		i += 2;
	// 	}
	// 	else if (input[i] == '>' && input[i + 1] == '>')
	// 	{
	// 		// to add the content - either by ft_strdup or st_substr
	// 		lexer->first_token->type == CHAR_APPEND;
	// 		i += 2;
	// 	}
	// 	else if (input[i] == '<' && input[i + 1] == '<')
	// 	{
	// 		// to add the content - either by ft_strdup or st_substr
	// 		lexer->first_token->type == CHAR_HEREDOC;
	// 		i += 2;
	// 	}
	// 	else if (input[i] == '|' && input[i + 1] == '|')
	// 	{
	// 		// to add the content - either by ft_strdup or st_substr
	// 		lexer->first_token->type == CHAR_OR;
	// 		i += 2;
	// 	}
	// }
	return (0);
}

int main()
{
	t_lexer	*lexer;
	char *test = "ls -l | grep \"this file\\\"\"";

	printf("%s\n", test);
	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (EXIT_FAILURE);
	lexer_function(test, lexer);
	return (0);
}