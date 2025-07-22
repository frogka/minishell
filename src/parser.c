#include "../includes/minishell.h"

void	print_ast_node(t_ast *node)
{
	printf("===============\n");
	printf("Node address: %p\n", node);
	printf("Node type: %i\n", node->type);
	printf("Node content: %s\n", node->content);
	printf("left node: %p\n", node->left);
	printf("right node: %p\n", node->right);
	printf("===============\n\n");
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
	node->right = r_node;
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

	// printf("\nThis is inside the consume first node function: %s\n", par->curr_token->content);
	if (is_default_token(par->curr_token->type))
	{
		output_node = create_ast_node(CHAR_DEF, par->curr_token->content);
		// *(par->root) = output_node;
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

void	ast_node_addback(t_ast *l_node, t_token *token)
{
	t_ast	*first_l_node;

	if (l_node->left == NULL)
	{
		l_node->left = create_ast_node(token->type, token->content);
		return ;
	}
	first_l_node = l_node->left;
	while (l_node->left != NULL)
		l_node = l_node->left;
	l_node->left = create_ast_node(token->type, token->content);
	l_node = first_l_node;
}

void	print_node_leafs(t_ast *node)
{
	t_ast	*l_node;
	t_ast	*r_node;

	print_ast_node(node);
	l_node = node->left;
	r_node = node->right;
	while (l_node != NULL)
	{
		printf("This is a left node: %s\n", l_node->content);
		l_node = l_node->left;
	}
	while (r_node != NULL)
	{
		printf("This is a left node: %s\n", r_node->content);
		r_node = r_node->left;
	}
}

void print_ast_sexpr(t_ast *root)
{
	if (!root)
	{
		printf("()\n");
		return;
	}
	
	ast_to_sexpr(root);
	printf("\n");
}

void ast_to_sexpr(t_ast *node)
{
	if (!node)
		return;

	if (!node->left && !node->right)
	{
		if (node->content)
			printf("%s", node->content);
		return;
	}
	printf("(");
	if (node->content)
		printf("%s", node->content);
	if (node->left)
	{
		printf(" ");
		ast_to_sexpr(node->left);
	}
	if (node->right)
	{
		printf(" ");
		ast_to_sexpr(node->right);
	}
	printf(")");
}

t_ast	*parser_function(t_parser *par, int min_bp)
{
	t_ast		*l_node;
	t_ast		*r_node;
	int			l_bp;
	int			r_bp;
	t_token		*op;

	if (par == NULL || par->initial_token == NULL)
		return (NULL);
	l_node = NULL;
	r_node = NULL;
	l_node = consume_first_node(par, &r_bp, r_node);
	if (l_node == NULL)
		printf("We have an error\n");
	while (1)
	{
		if (par->curr_token == NULL)
			break ;
		else if (is_default_token(par->curr_token->type) && is_default_token(l_node->type))
		{
			ast_node_addback(l_node, par->curr_token);
			par->curr_token = par->curr_token->next;
			continue;
		}
		// printf("Type: %i; Content:%s \n", par->curr_token->type, par->curr_token->content);
		l_bp = infix_binding_power(par->curr_token->type, LEFT);
		r_bp = infix_binding_power(par->curr_token->type, RIGHT);
		if(l_bp != -1 && r_bp != -1)
		{
			// printf("in Here. This is l_bp: %i and r_bp: %i\n", l_bp, r_bp);
			if (l_bp < min_bp)
			{
				break;
			}
			op = par->curr_token;
			// printf("This is the address of curr_token: %p; this is the content: %s\n", op, op->content);
			par->curr_token = par->curr_token->next;
			r_node = parser_function(par, r_bp);
			print_node_leafs(l_node);
			print_ast_node(r_node);
			printf("This is the address of curr_token: %p; this is the content: %s\n", op, op->content);
			l_node = create_ast_structure(op, l_node, r_node);
			continue;
		}
		break;
	}
	return (l_node);
}
