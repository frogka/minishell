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

t_ast	*create_ast_structure(t_token *token, t_ast *l_node, t_ast *r_node)
{
	t_ast	*node;
	
	node = create_ast_node(token->type, token->content);
	node->left = l_node;
	node->left = r_node;
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

int	is_default_token(int type)
{
	if (type == CHAR_DEF
		|| type == CHAR_QUOTE
		|| type == CHAR_DQUOTE)
	{
		return (1);
	}
	else
		return (0);
}

int	is_redirect_token(int type)
{
	if (type == CHAR_INRED
		|| type == CHAR_OUTRED
		|| type == CHAR_APPEND
		|| type == CHAR_HEREDOC)
	{
		return (1);
	}
	else
		return (0);
}

int	is_operator_token(int type)
{
	if (type == CHAR_PIPE
		|| type == CHAR_AMPERSAND
		|| type == CHAR_DOLLAR
		|| type == CHAR_QM
		|| type == CHAR_AND
		|| type == CHAR_OR)
	{
		return (1);
	}
	else
		return (0);
}

t_ast	*consume_first_node(t_parser *par, int *r_bp, t_ast *r_node)
{
	t_ast	*output_node;

	if (is_default_token(par->curr_token->type))
	{
		output_node = create_ast_node(CHAR_DEF, par->curr_token->content);
		*(par->root) = output_node;
		par->curr_token = par->curr_token->next;
		return (output_node);
	}
	else if (is_redirect_token(par->curr_token->type))
	{
		*r_bp = prefix_binding_power(par->curr_token->type, RIGHT);
		r_node = parser_function(par, *r_bp);
		output_node = create_ast_structure(par->curr_token, NULL, r_node);
		par->curr_token = par->curr_token->next;
		return (output_node);
	}
	else
		return (NULL);
}

int	infix_binding_power(int type, int side)
{
	if ((type == ';' || type == '&') && side == LEFT)
		return (1);
	else if ((type == ';' || type == '&')  && side == RIGHT)
		return (2);
	else if ((type == CHAR_AND || type == CHAR_OR) && side == LEFT)
		return (3);
	else if ((type == CHAR_AND || type == CHAR_OR) && side == RIGHT)
		return (4);
	else if (type == CHAR_PIPE && side == LEFT)
		return (5);
	else if (type == CHAR_PIPE && side == RIGHT)
		return (6);
	else if (is_redirect_token(type) && side == LEFT)
		return (0);
	else if (is_redirect_token(type) && side == RIGHT)
		return (8);
	else
		return (-1);
}

int	prefix_binding_power(int type, int side)
{
	if (is_redirect_token(type) && side == LEFT)
		return (0);
	else if (is_redirect_token(type) && side == RIGHT)
		return (8);
	else
		return (-1);
}

t_ast	*parser_function(t_parser *par, int min_bp)
{
	t_ast		*l_node;
	t_ast		*r_node;
	int			l_bp;
	int			r_bp;

	if (par == NULL || par->initial_token == NULL)
		return (NULL);
	l_node = NULL;
	r_node = NULL;
	l_node = consume_first_node(par, &r_bp, r_node);
	if (l_node == NULL)
		printf("We have an error\n");
	while (1)
	{
		if (par->curr_token->next == NULL)
			break ;
		printf("Type: %i; Content:%s \n", par->curr_token->next->type, par->curr_token->next->content);
		l_bp = infix_binding_power(par->curr_token->next->type, LEFT);
		r_bp = infix_binding_power(par->curr_token->next->type, RIGHT);
		printf("l_bp: %i and r_bp: %i\n", l_bp, r_bp);
		if(l_bp != -1 && r_bp != -1)
		{
			printf("in Here\n");
			if (l_bp < min_bp)
			{
				break;
			}
			par->curr_token = par->curr_token->next;
			r_node = parser_function(par, r_bp);
			l_node = create_ast_structure(par->curr_token, l_node, r_node);
			continue;
		}
		break;
	}
	return (l_node);
}
