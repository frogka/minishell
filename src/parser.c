#include "../includes/minishell.h"

void	print_ast_node(t_ast *node)
{
	printf("Node address: %p\n", node);
	printf("Node type: %i\n", node->type);
	printf("Node content: %s\n", node->content);
	printf("left node: %p\n", node->left);
	printf("right node: %p\n", node->right);
}

void	ast_token_next(t_parser *par)
{
	par->curr_token = par->curr_token->next;
}

t_ast	*create_ast_node(int type, char *content)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->content = ft_strdup(content);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_parser	*init_paser(t_lexer *lex)
{
	t_parser *par;

	par = malloc(sizeof(t_parser));
	par->root = malloc(sizeof(t_ast *));
	if (par == NULL || par->root == NULL)
		return (NULL);
	par->initial_token = lex->first_token;
	par->curr_token = lex->first_token;
	return(par);
}

int	is_default_token(t_token *token)
{
	if (token->type == CHAR_DEF
		|| token->type == CHAR_QUOTE
		|| token->type == CHAR_DQUOTE)
	{
		return (1);
	}
	else
		return (0);
}

int	parser_function(t_lexer *lex/*, int min_bp*/)
{
	t_parser	*par;
	// int			l_bp;
	// int			r_bp;

	if (lex == NULL || lex->first_token == NULL)
		return (1);
	par = init_paser(lex);
	if (is_default_token(par->curr_token))
		*(par->root) = create_ast_node(CHAR_DEF, lex->first_token->content);
	else
		*(par->root) = create_ast_node(lex->first_token->type, lex->first_token->content);
	while (1)
	{
		if (par->curr_token->next == NULL)
			break;
		// else if (par->curr_token->next->type == CHAR_DEF
		// 			|| par->curr_token->next->type = CHAR_DQUOTE
		// 			|| par->curr_token->next->type = CHAR_QUOTE)
		// {

		// }
		
	}
	


	*(par->root) = create_ast_node(lex->first_token->type, lex->first_token->content);
	print_ast_node(*(par->root));
	return (0);
}
