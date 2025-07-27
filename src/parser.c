#include "../includes/minishell.h"

/* Start of AUX functions */

void	print_ast_node(t_ast *node)
{
	if (node == NULL)
		return;	
	printf("===============\n");
	printf("Node address: %p\n", node);
	printf("Node type: %i\n", node->type);
	printf("Node content: %s\n", node->data);
	printf("left node: %p\n", node->left);
	printf("right node: %p\n", node->right);
	printf("===============\n\n");
}

void	print_ast_sexpr(t_ast *root)
{
	if (!root)
	{
		printf("()\n");
		return;
	}
	
	ast_to_sexpr(root);
	printf("\n");
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
		printf("This is a left node: %s\n", l_node->data);
		l_node = l_node->left;
	}
	while (r_node != NULL)
	{
		printf("This is a left node: %s\n", r_node->data);
		r_node = r_node->left;
	}
}

void	ast_to_sexpr(t_ast *node)
{
	if (!node)
		return;

	if (!node->left && !node->right)
	{
		if (node->data)
			printf("%s", node->data);
		return;
	}
	printf("(");
	if (node->data)
		printf("%s", node->data);
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

t_ast	*create_ast_node(int type, char *content)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->data = ft_strdup(content);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast	*create_ast_structure(t_token *token, t_ast *l_node, t_ast *r_node)
{
	t_ast	*node;
	
	node = create_ast_node(token->type, token->data);
	node->left = l_node;
	node->right = r_node;
	return (node);
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

void	ast_node_addback(t_ast *l_node, t_token *token)
{
	t_ast	*first_l_node;

	if (l_node->left == NULL)
	{
		l_node->left = create_ast_node(token->type, token->data);
		return ;
	}
	first_l_node = l_node->left;
	while (l_node->left != NULL)
		l_node = l_node->left;
	l_node->left = create_ast_node(token->type, token->data);
	l_node = first_l_node;
}

void	ast_node_placeback(t_ast **node_root, t_ast *node_toadd, int side)
{
	t_ast	*first_node;

	if (*node_root == NULL)
	{
		*node_root = node_toadd;
		return ;
	}
	if (side == LEFT)
	{
		if ((*node_root)->left == NULL)
		{
			(*node_root)->left = node_toadd;
			return ;
		}
		first_node = (*node_root)->left;
		while ((*node_root)->left->left)
			(*node_root)->left = (*node_root)->left->left;
		(*node_root)->left->left = node_toadd;
		(*node_root)->left = first_node; 
	}
	else
	{
		if ((*node_root)->right == NULL)
		{
			(*node_root)->right = node_toadd;
			return ;
		}
		first_node = (*node_root)->right;
		while ((*node_root)->right->right)
			(*node_root)->right = (*node_root)->right->right;
		(*node_root)->right->right = node_toadd;
		(*node_root)->right = first_node; 
	}
}

/* End of AUX functions */

/* Start of Free functions */

void	free_ast(t_ast *root)
{
	if (root)
	{
		free_ast(root->left);
		free_ast(root->right);
		free(root->data);
		free(root);
	}
}

/* TODO: To check if this is the source of the double free of $USER */
void	free_parser_struct(t_parser *par)
{
	if (par)
	{
		free(par->root);
		free(par);
	}
	
}

/* End of Free functions*/

/* Start Parsing Functions*/

void	ast_token_next(t_parser *par)
{
	par->curr_token = par->curr_token->next;
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

void	infix_binding_power(int type, t_bp *bp)
{
	if (type == ';' || type == '&')
	{
		bp->l = 1;
		bp->r = 2;
	}
	else if (type == CHAR_AND || type == CHAR_OR)
	{
		bp->l = 3;
		bp->r = 4;
	}
	else if (type == CHAR_PIPE)
	{
		bp->l = 5;
		bp->r = 6;
	}
	else
	{
		bp->l = -1;
		bp->r = -1;
	}
}

t_ast	*parse_simple_command(t_parser *par)
{
	t_ast	*file;
	t_ast	*cmd;
	t_ast	*redi;
	t_ast	*redi_root;
	t_token	*token_redirect;

	file = NULL;
	cmd = NULL;
	redi = NULL;
	redi_root = NULL;

	while (par->curr_token && !is_operator_token(par->curr_token->type))
	{
		if (is_redirect_token(par->curr_token->type))
		{
			token_redirect = par->curr_token;
			par->curr_token = par->curr_token->next;
			if (!is_default_token(par->curr_token->type))
			{
				printf("Error: syntax error near unexpecter token `newline'");
				return (NULL);
			}
			file = create_ast_node(CHAR_DEF, par->curr_token->data);
			redi = create_ast_structure(token_redirect, NULL, file);
			ast_node_placeback(&redi_root, redi, RIGHT);
			par->curr_token = par->curr_token->next;
		}
		else if(is_default_token(par->curr_token->type))
		{
			if (cmd == NULL)
				cmd = create_ast_node(CHAR_DEF, par->curr_token->data);
			else
				ast_node_addback(cmd, par->curr_token);
			par->curr_token = par->curr_token->next;
		}
		else
			break;
	}
	if (cmd == NULL && redi_root != NULL)
		cmd = redi_root;
	else if (cmd != NULL && redi_root != NULL)
		cmd->right = redi_root;
	return (cmd);	
}

void	parser_function_loop(t_parser *par, int min_bp, t_ast **l_node, t_ast **r_node)
{
	t_bp		bp;
	t_token		*op;

	while (1)
	{
		if (par->curr_token == NULL)
			break ;
		else if (is_default_token(par->curr_token->type) && is_redirect_token((*l_node)->type))
		{
			ast_node_addback(*r_node, par->curr_token);
			par->curr_token = par->curr_token->next;
			break;
		}
		infix_binding_power(par->curr_token->type, &bp);
		if(bp.l != -1 && bp.r != -1)
		{
			if (bp.l < min_bp)
				break;
			op = par->curr_token;
			par->curr_token = par->curr_token->next;
			*r_node = parser_function(par, bp.r);
			*l_node = create_ast_structure(op, *l_node, *r_node);
			continue;
		}
		break;
	}
}

t_ast	*parser_function(t_parser *par, int min_bp)
{
	t_ast		*l_node;
	t_ast		*r_node;

	if (par == NULL || par->initial_token == NULL)
		return (NULL);
	l_node = NULL;
	r_node = NULL;
	l_node = parse_simple_command(par);
	if (l_node == NULL)
	{
		// printf("minishell: syntax error near unexpected token `%s'\n", par->curr_token->content);
		return (NULL);
	}
	parser_function_loop(par, min_bp, &l_node, &r_node);
	return (l_node);
}

/* End Parsing functions */