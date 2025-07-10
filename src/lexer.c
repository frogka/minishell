#include "../includes/minishell.h"

int lexer(char *input, t_lexer	*lexer)
{
	int	i;
	int	len_input;
	
	i = 0;
	len_input = ft_strlen(input);
	// need to create first Token for malloc if there are no major errors is correct
	// need to check if there are matching quotes in the input
	while (input[i])
	{
		if (input[i] == '&' && input[i + 1] == '&')
		{
			// to add the content - either by ft_strdup or st_substr
			lexer->first_token->type == CHAR_AND;
			i += 2;
		}
		else if (input[i] == '>' && input[i + 1] == '>')
		{
			// to add the content - either by ft_strdup or st_substr
			lexer->first_token->type == CHAR_APPEND;
			i += 2;
		}
		else if (input[i] == '<' && input[i + 1] == '<')
		{
			// to add the content - either by ft_strdup or st_substr
			lexer->first_token->type == CHAR_HEREDOC;
			i += 2;
		}
		else if (input[i] == '|' && input[i + 1] == '|')
		{
			// to add the content - either by ft_strdup or st_substr
			lexer->first_token->type == CHAR_OR;
			i += 2;
		}
	}
	
}

int main()
{
	t_lexer	*lexer;
	char *test = "ls -l | grep \"this file\"";

	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (EXIT_FAILURE);
	lexer(test, lexer);
	return (0);
}