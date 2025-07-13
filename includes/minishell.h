#ifndef MINISHELL_H
#define MINISHELL_H
#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include "../libft/libft.h"
#define IN_QUOTES 1
#define DEF 0

typedef	struct s_ast
{
	int	type;
	char *content;
	struct s_ast *left;
	struct s_ast *right;
}	t_ast;

enum e_token_type
{
	TOKEN = -1,
	CHAR_DEF = -1,
	CHAR_PIPE = '|',
	CHAR_AMPERSAND = '&',
	CHAR_DOLLAR = '$',
	CHAR_QM = '?',
	CHAR_ESCAPE = '\\',
	CHAR_QUOTE = '\'',
	CHAR_DQUOTE = '\"',
	CHAR_OUTRED = '>',
	CHAR_INRED = '<',
	CHAR_SPACE = ' ',
	CHAR_TAB = '\t',
	CHAR_NEWLINE = '\n',
	CHAR_NULL = 0,
	CHAR_APPEND = 256,
	CHAR_HEREDOC,
	CHAR_AND,
	CHAR_OR
};

typedef struct s_token
{
	int	type; // substitute by an enum afterwards
	char *content;
	struct s_token *next;
}	t_token;

typedef struct s_lexer
{
	t_token *first_token;
	int		count_token;
}	t_lexer;

typedef struct s_token_aux
{
	int	i;
	int	j;
	int	len_input;
	t_token *curr_token;
	int status;
}	t_token_aux;


#endif