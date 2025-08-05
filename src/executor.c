#include "../includes/minishell.h"

/* TODOS
SHOULD BE DONE ===> Add error handling functions
===> Check the following bug

((echo this && echo that) || echo cona) | grep this
(| (|| (&& (echo this) (echo that)) (echo cona)) (grep this))
[124] |
[259] ||
[258] &&
[-1] echo
[-1] echo
that
[-1] grep
$>Duplicating read pipe to STDIN: Bad file descriptor

*/

/* Start of Redirections */

int	open_fd(char *path, int option, t_px *px)
{
	int	fd;

	fd = -1;
	if (option == CHAR_INRED)
		fd = open(path, O_RDONLY);
	else if (option == CHAR_OUTRED)
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (option == CHAR_HEREDOC)
		fd = heredoc(path, px);
	else if (option == CHAR_APPEND)
		fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd == -1 && CHAR_INRED == 'I')
		perror("Error: opening file");
	return (fd);
}

int	write_line(char *limit, int fd, int fd_stdout)
{
	char	*line;
	char	*limitor;
	size_t	size;

	limitor = ft_strjoin(limit, "\n");
	size = ft_strlen(limitor);
	while (1)
	{
		write(fd_stdout, "> ", 2);
		line = get_next_line(0);
		if (size == ft_strlen(line) && ft_strncmp(limitor, line, size) == 0)
		{
			free(line);
			free(limitor);
			close(fd);
			get_next_line(-1);
			exit(EXIT_SUCCESS);
		}
		if (write(fd, line, ft_strlen(line)) == -1)
			error_handler("Writing lines", NULL, 1, NULL);
		free(line);
	}
	exit(EXIT_FAILURE);
}

int	heredoc(char *limit, t_px *px)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		error_handler("Laying down the pipe(s)", NULL, 1, NULL);
	pid = fork();
	if (pid == -1)
		error_handler("Fork creation", NULL, 1, NULL);
	if (pid == 0)
	{
		close(pipe_fd[READ]);
		write_line(limit, pipe_fd[WRITE], px->fd_stdout);
	}
	else
	{
		close(pipe_fd[WRITE]);
		waitpid(pid, NULL, 0);
		return (pipe_fd[READ]);
	}
	return (-1);
}

void	redirections_files_setup(int fd, int type, int num_output_fd)
{
	if (fd == -1)
		return ;
	if (type == CHAR_INRED || type == CHAR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			error_handler("Duplicating read-end pipe to STDIN", NULL, 1, NULL);
		close(fd);
	}
	else
	{
		if (num_output_fd == 0)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				error_handler("Duplicating write-end pipe to STDOUT", NULL, 1, NULL);
		}
		close(fd);
	}
}

int	redirections_setup(t_ast *root, t_px *px)
{
	int	fd;
	int	num_output_fd;

	num_output_fd = 0;
	while (root)
	{
		if (is_redirect_token(root->type))
		{
			fd = open_fd(root->right->data, root->type, px);
			if (fd == -1)
			{
				restore_fd(px);
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(root->right->data, STDERR_FILENO);
				ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
				return (EXIT_FAILURE);
			}
			redirections_files_setup(fd, root->type, num_output_fd);
		}
		if (root->type == CHAR_OUTRED || root->type == CHAR_APPEND)
			num_output_fd++;
		root = root->right;
	}
	return (EXIT_SUCCESS);
}

/* End of Redirections */

/* Start of Initializations */

int	count_number_commands(t_ast *root_tree)
{
	int	total;

	total = 0;
	if (root_tree == NULL)
		return (0);
	if (is_default_token(root_tree->type))
		total++;
	else if (is_operator_token(root_tree->type))
	{
		total += count_number_commands(root_tree->left);
		total += count_number_commands(root_tree->right);
	}
	return (total);
}

t_px	*initialize_px(t_ast *root_tree)
{
	t_px	*px;

	px = malloc(sizeof(t_px));
	malloc_error_handler(px, EXIT_FAILURE);
	px->num_commands = count_number_commands(root_tree);
	px->root_tree = root_tree;
	px->fd_stdin = dup(STDIN_FILENO);
	px->fd_stdout = dup(STDOUT_FILENO);
	return (px);
}

/* End of Initializations*/

/* Start of Executor functions */

/* executes a subtree */
int	executor_aux(t_px *px, t_ast *root)
{
	int	exit_code;

	if (root == NULL)
		return (EXIT_SUCCESS);
	if (is_default_token(root->type))
	{ 
		exit_code = executor(px, root);
		return (exit_code);
	}
	else if (root->type == CHAR_PIPE)
	{
		return (executor_pipe(px, root));
	}
	else if (root->type == CHAR_AMPERSAND || root->type == CHAR_DOLLAR
			|| root->type == CHAR_QM) // To check if we can remove
	{
		executor_aux(px, root->left);
		executor_aux(px, root->right);
	}
	else if (root->type == CHAR_AND)
	{
		exit_code = executor_aux(px, root->left);
		if (exit_code != 0)
			return (exit_code);	
		return (executor_aux(px, root->right));
	}
	else if(root->type == CHAR_OR)
	{
		exit_code = executor_aux(px, root->left);
		if (exit_code == 0)
			return (exit_code);
		return (executor_aux(px, root->right));
	}
	return (EXIT_SUCCESS);
}

void	restore_fd(t_px *px)
{
	dup2(px->fd_stdin, STDIN_FILENO);
	dup2(px->fd_stdout, STDOUT_FILENO);
	close(px->fd_stdin);
	close(px->fd_stdout);
}

int	executor_pipe(t_px *px, t_ast *root)
{
	int	pipe_fd[2];
	int	pids[2];
	int	status;
	int	exit_code;

	status = 0;
	if(pipe(pipe_fd) != 0)
		error_handler("Laying down the pipe(s)", NULL, 1, NULL);
	pids[0] = fork();
	if (pids[0] == 0)
	{
		child_signals();
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		close(pipe_fd[READ]);
		exit_code = execute_subshell(px, root->left);
		exit (exit_code);
	}
	ignore_signals();
	pids[1] = fork();
	if (pids[1] == 0)
	{
		child_signals();
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[WRITE]);
		exit_code = execute_subshell(px, root->right);
		exit (exit_code);
	}
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
	waitpid(pids[0], &status, 0);
	if (WIFSIGNALED(status))
		write(1, "\n",1);
	waitpid(pids[1], &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (WTERMSIG(status) + 128);
	}
	return (EXIT_FAILURE);
}

int	execute_subshell(t_px *px, t_ast *root)
{
	t_px	*px_subshell;
	int		exit_code;

	px_subshell = initialize_px(root);
	px_subshell->fd_stdin = px->fd_stdin;
	px_subshell->fd_stdout = px->fd_stdout;
	exit_code = executor_aux(px_subshell, root);
	free(px_subshell);
	return (exit_code);
}

int	executor(t_px *px, t_ast *cmd_node)
{
	int	status;
	int	pid;

	pid = fork();
	status = 0;
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		child_signals();
		status = redirections_setup(cmd_node, px);
		if (status == EXIT_FAILURE)
			exit (EXIT_FAILURE);		
		if (cmd_node->data == NULL || cmd_node->data[0] == 0)
			error_handler("No command ''", NULL, 1, px);
		if (is_default_token(cmd_node->type))
			exec_command(px, cmd_node);
	}
	ignore_signals();
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (WTERMSIG(status) + 128);
	}
	return (EXIT_FAILURE);
}

/* Executes a subtree that is only composed out of a builtin function. */
int	executor_builtin_func(t_px *px)
{
	int	exit_code;

	exit_code = redirections_setup(px->root_tree, px);
	if (exit_code == EXIT_SUCCESS)
		exit_code = builtin_functions(px->root_tree, NULL, px, TO_RETURN);
	dup2(px->fd_stdin, STDIN_FILENO);
	dup2(px->fd_stdout, STDOUT_FILENO);
	close(px->fd_stdin);
	close(px->fd_stdout);
	free_px(px);
	return (exit_code);
}

void	exec_command_free_aux(char **paths, char **commands, t_px *px)
{
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	free_arrays(commands);
	free_arrays(paths);
	free_px(px);
	free_struct_to_free();
	free_global_struct();
	rl_clear_history();
	free(pl->prompt);
}

void	exec_command(t_px *px, t_ast *cmd_node)
{
	int		j;
	char	**paths;
	char	*final_path;
	char	**commands;

	commands = commands_extractor(cmd_node);
	builtin_functions(cmd_node, commands, px, TO_EXIT);
	paths = path_extractor();
	if (paths == NULL)
		error_handler("Error: problem envp file path", NULL, 1, NULL);
	j = 0;
	while (paths[j])
	{
		final_path = ft_strjoin_3(paths[j], '/', commands[0]);
		if (access(final_path, F_OK) == 0)
			execve_checker(final_path, commands, paths, px);
		free (final_path);
		j++;
	}
	if (access(commands[0], F_OK) == 0)
		execve_checker(NULL, commands, paths, px);
	exec_command_free_aux(paths, commands, px);
	error_handler("command not found", NULL, 127, NULL);
}

void	execve_checker(char *f_path, char **comms, char **paths, t_px *px)
{
	t_global *global;

	global = global_struct();
	if (execve(f_path, comms, global->ev) == -1 && f_path != NULL)
	{
		free(f_path);
		free_arrays(comms);
		free_arrays(paths);
		error_handler("execve call:", NULL, 1, px);
	}
	else if (execve(comms[0], comms, global->ev) == -1 && f_path == NULL)
	{
		free_arrays(comms);
		free_arrays(paths);
		error_handler("execve call:", NULL, 1, px);
	}
}

int executor_function(t_ast *root_tree)
{
	t_px	*px;
	int		exit_code;
	
	if (root_tree == NULL)
		return (EXIT_FAILURE);
	px = initialize_px(root_tree);
	if (is_builtin(px->root_tree))
		return (executor_builtin_func(px));
	if (px->num_commands == 0)
	{
		exit_code = redirections_setup(px->root_tree, px);
		restore_fd(px);
		free_px(px);
		return (exit_code);
	}
	else
	{
		exit_code = executor_aux(px, px->root_tree);
		free_px(px);
		return (exit_code);
	}
}

/* End of Executor Functions */

/* Executor AUX functions */

void	error_handler(char *msg, char *file_name, int error_code, t_px *px)
{
	char	*err_msg;

	if (file_name == NULL && px == NULL)
	{
		perror(msg);
		exit(error_code);
	}
	else if (file_name == NULL && px != NULL)
	{
		perror(msg);
		free_px(px);
		exit(error_code);
	}
	else
	{
		err_msg = ft_strjoin("./minishell: ", file_name);
		perror(err_msg);
		free(err_msg);
		exit(error_code);
	}
}

void	malloc_error_handler(void *ptr, int error_code)
{
	if (!ptr)
		error_handler("Error with malloc", NULL, error_code, NULL);
}

char	**commands_extractor(t_ast *cmd_node)
{
	t_ast	*temp;
	int		count;
	char	**commands;

	if (cmd_node == NULL || cmd_node->data == NULL
			|| cmd_node->type != CHAR_DEF)
		return (NULL);	
	temp = cmd_node;
	count = 0;
	while (temp)
	{
		count++;
		temp = temp->left;
	}
	commands = malloc(sizeof(char *) * (count + 1));
	temp = cmd_node;
	count = 0;
	while (temp)
	{
		commands[count] = ft_strdup(temp->data);
		temp = temp->left;
		count++;
	}
	commands[count] = NULL;
	return (commands);
}

char	**path_extractor(void)
{
	char		**paths;
	t_global	*global;
	int			i;
	
	global = global_struct();
	i = -1;
	while (global->ev[++i])
	{
		if (ft_strncmp(global->ev[i], "PATH=", 4) == 0)
		{
			paths = ft_split(global->ev[i] + 5, ':');
			return (paths);
		}
	}
	return (NULL);
}

char	*ft_strjoin_3(const char *s1, char connector, const char *s2)
{
	int		i;
	int		j;
	char	*res;

	res = malloc((ft_strlen(s1) + 2 + ft_strlen(s2)) * sizeof(char));
	if (!res)
		error_handler("Malloc problem in ft_strjoin_3", NULL, 1, NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = connector;
	i++;
	j = 0;
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = 0;
	return (res);
}

void	free_arrays(char **arrays)
{
	int	i;

	i = 0;
	while (arrays[i])
	{
		free(arrays[i]);
		i++;
	}
	free(arrays);
}

void	free_px(t_px *px)
{
	free(px);
}

void	free_struct_to_free(void)
{
	t_to_free	*to_free;

	to_free = to_free_struct();
	free_lexer(to_free->lexer);
	free_ast(to_free->root_tree);
	free_parser_struct(to_free->par);
}

/* END of Executor AUX functions*/
