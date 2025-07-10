#include "minishell.h"

typedef	struct s_ast
{
	int	type;
	char *content;
	struct s_ast *left;
	struct s_ast *right;
}	t_ast;

void	print_ast_node(t_ast *node)
{
	printf("Node address: %p\n", node);
	printf("Node type: %i\n", node->type);
	printf("Node content: %s\n", node->content);
	printf("left node: %p\n", node->left);
	printf("right node: %p\n", node->right);
}

// int	main(void)
// {
// 	t_ast node;

// 	node.content = "2";
// 	node.type = 0;
// 	node.left = NULL;
// 	node.right = NULL;
// 	print_ast_node(&node);
// 	return (0);
// }
