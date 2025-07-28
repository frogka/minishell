#ifndef MINISHELL_H
#define MINISHELL_H
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../libft/libft.h"
#define DEF 0
#define LEFT 0
#define RIGHT 1
#define READ 0
#define WRITE 1
#define	NO_BUILTIN 69420
#define TO_RETURN 0
#define TO_EXIT 1

typedef	struct s_ast
{
	int				type;
	char 			*data;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_global
{
	int		exit_code;
	char	**ev;
}	t_global;

typedef struct s_bp
{
	int	l;
	int	r;
}	t_bp;

typedef struct s_px
{
	pid_t	*pids;
	int		**pipes;
	int		num_pipes;
	int		num_commands;
	int		curr_index;
	int		fd_stdout;
	int		fd_stdin;
	t_ast	*root_tree;
}	t_px;

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
	int				type; // substitute by an enum afterwards
	char 			*data;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	t_token *first_token;
	int		count_token;
}	t_lexer;

typedef struct s_token_aux
{
	int		i;
	int		j;
	int		len_input;
	t_token *curr_token;
	int		status;
}	t_token_aux;

typedef struct s_parser
{
	t_ast 	**root;
	t_token	*initial_token;
	t_token *curr_token;
	int		count_token;
}	t_parser;

typedef	struct s_to_free
{
	t_parser	*par;
	t_ast		*root_tree;
	t_lexer		*lexer;
}	t_to_free;

typedef	struct s_prompt_line
{
	char	*prompt;
	char	*line;
}	t_prompt_line;

/* lexer.c */
t_global *global_struct(void);
t_to_free	*to_free_struct(void);
int	check_matching_quotes(char *input);
t_token	*get_last_token(t_lexer *lexer);
t_token	*get_previous_token(t_token *first_token, t_token *curr_token);
t_token	*init_token(t_token *token, int len_input);
t_token	*add_token_back(t_lexer *lexer, int len_input);
void	handle_def_1char(char *input, t_token_aux *aux, t_lexer *lexer, int *f);
void	handle_quote_1char(char *input, t_token_aux *aux, t_lexer *lexer, int *f);
void	handle_def_2char(char *input, t_token_aux *aux, t_lexer *lexer, int *f);
void	handle_terminal(char *input, t_token_aux *aux, t_lexer *lexer, int *f);
void	handle_start_quote(char *input, t_token_aux *aux, t_lexer *lexer, int *f);
void	process_char_def(char *input, t_token_aux *aux, t_lexer *lexer);
void	process_char_quote(char *input, t_token_aux *aux, t_lexer *lexer);
void	process_char_dquote(char *input, t_token_aux *aux, t_lexer *lexer);
int	check_only_terminal(char *input);
void	init_lexer_aux(char *input, t_token_aux *aux, t_lexer *lexer);
void	process_char(char *input, t_token_aux *aux, t_lexer *lexer);
void	clean_last_tokens(t_token_aux *aux, t_lexer *lexer);
char	*find_ev(char *to_expand);
void	insert_expansion(t_token *token, int sta, int len, char *mid_str);
void	token_expansion_aux(t_token *token);
void	token_expansion(t_token_aux *aux, t_lexer *lexer);
int	lexer_function(char *input, t_lexer *lexer);

/* parser.c */
void	print_ast_node(t_ast *node);
void	ast_token_next(t_parser *par);
t_ast	*create_ast_node(int type, char *content);
t_parser	*init_paser(t_lexer *lex);
void	free_parser_struct(t_parser *par);
int	is_default_token(int type);
int	is_redirect_token(int type);
int	is_operator_token(int type);
void	infix_binding_power(int type, t_bp *bp);
int	prefix_binding_power(int type, int side);
t_ast	*parser_function(t_parser *par, int min_bp);
t_ast	*parse_simple_command(t_parser *par);


void print_ast_sexpr(t_ast *root);
void ast_to_sexpr(t_ast *node);
void	free_ast(t_ast *root);

/* aux.c */
void	init_ev(char *envp[]);
void	init_global_struct(char *envp[]);
void	free_global_struct(void);
t_lexer	*init_lexer(void);
void	free_lexer(t_lexer *lexer);

/* executor.c */
int	open_fd(char *path, int option, t_px *px);
int	write_line(char *limit, int fd, int fd_stdout);
int	heredoc(char *limiter, t_px *px);
int	count_number_commands(t_ast *root_tree);
int	count_number_pipes(t_ast *root_tree);
t_px	*initialize_px(t_ast *root_tree);
void	create_pipeline(t_px *px);
void	executor_aux(t_px *px, t_ast *root);
void	child_pipe_setup(t_px *px, int i);
int	executor(t_px *px, int i, t_ast *cmd_node);
void	exec_command(t_px *px, t_ast *cmd_node);
char	**commands_extractor(t_ast *cmd_node);
char	**path_extractor(void);
void	free_arrays(char **arrays);
void	free_px(t_px *px);
char	*ft_strjoin_3(const char *s1, char connector, const char *s2);
void	execve_checker(char *f_path, char **comms, char **paths, t_px *px);
int executor_function(t_ast *root_tree);
void	error_handler(char *msg, char *file_name, int error_code, t_px *px);
void	malloc_error_handler(void *ptr, int error_code);
void	free_struct_to_free(void);

/* terminal.c */
void	terminal();

/* builtin.c */
int		pwd_builtin(void);
int		builtin_execution(t_ast *node);
int		builtin_functions(t_ast *node, char **comms, t_px *px, int to_exit);
int		is_builtin(t_ast *n);
int		echo_builtin(t_ast *node);
int		cd_builtin(t_ast *node);
int		exit_builtin(void);
int		export_builtin(t_ast *node);
void	remove_env(char *env_to_remove);
void	remove_env_aux(int count);
int		unset_builtin(t_ast *node);
int		env_builtin(t_ast *node);
t_prompt_line	*to_prompt_line_struct(void);

/* env.c */
void	add_env(char *to_add);
void	update_env(char *env_to_change, char *new_env, char *to_free);

#endif