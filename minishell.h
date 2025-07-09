#include <signal.h>
#include <stdio.h>
#include <strings.h>

typedef	struct s_ast
{
	int	type;
	char *content;
	struct s_ast *left;
	struct s_ast *right;
}	t_ast;

typedef struct s_token
{
	int	type; // substitute by an enum afterwards
	char *content;
	struct s_token *next;
}	t_token;
