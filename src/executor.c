#include "../includes/minishell.h"

int	open_fd(char *path, int option)
{
	int	fd;

	fd = -1;
	if (option == CHAR_INRED)
		fd = open(path, O_RDONLY);
	else if (option == CHAR_OUTRED)
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (option == CHAR_HEREDOC)
	{
		/* code */
	}
	else if (option == CHAR_APPEND)
		fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd == -1 && option == 'I')
		perror("Error: opening file");
	return (fd);
}

int	count_number_commands(t_ast *root_tree)
{
	int	total;

	total = 0;
	if (root_tree == NULL)
		return (0);
	if (is_default_token(root_tree->type))
	{
		total++;
	}
	else if (is_operator_token(root_tree->type))
	{
		total += count_number_commands(root_tree->left);
		total += count_number_commands(root_tree->right);
	}
	return (total);
}

int	count_number_pipes(t_ast *root_tree)
{
	int	total;

	total = 0;
	if (root_tree == NULL)
		return (0);
	total += count_number_pipes(root_tree->left);
	total += count_number_pipes(root_tree->right);
	if (root_tree->type == '|')
		total++;
	return (total);
}

t_px	*initialize_px(t_ast *root_tree)
{
	t_px	*px;

	px = malloc(sizeof(t_px));
	// malloc_error_handler(px, EXIT_FAILURE);
	px->pids = NULL;
	px->pipes = NULL;
	px->num_commands = count_number_commands(root_tree);
	px->num_pipes = count_number_pipes(root_tree);
	px->root_tree = root_tree;
	px->curr_index = 0;
	px->pids = malloc(sizeof(pid_t) * px->num_commands);
	// malloc_error_handler(px->pids, EXIT_FAILURE);
	create_pipeline(px);
	return (px);
}

void	create_pipeline(t_px *px)
{
	int	i;

	if (px->num_pipes == 0)
		return ;	
	px->pipes = malloc(sizeof(int *) * (px->num_pipes));
	if (!px->pipes)
		printf("To Add error Handler function\n");
		// error_handler("malloc in pipe creation", NULL, EXIT_FAILURE, NULL);
	i = 0;
	while (i < px->num_pipes)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i])
			printf("To Add error Handler function\n");
			// error_handler("malloc in pipe creation", NULL, EXIT_FAILURE, NULL);
		if (pipe(px->pipes[i]) == -1)
			printf("To Add error Handler function\n");
			// error_handler("Pipe creation", NULL, EXIT_FAILURE, NULL);
		i++;
	}
}

void	child_pipe_setup(t_px *px, int i)
{
	if (i == 0)
	{
		if (dup2(px->pipes[0][WRITE], STDOUT_FILENO) == -1)
			printf("To Add error Handler function\n");
			// error_handler("Duplicating write pipe to STDOUT\n", NULL, 1, px);
	}
	else if (i < px->num_pipes)
	{
		if (dup2(px->pipes[i - 1][READ], STDIN_FILENO) == -1)
			printf("To Add error Handler function\n");
			// error_handler("Duplicating read pipe to STDIN", NULL, 1, px);
		if (dup2(px->pipes[i][WRITE], STDOUT_FILENO) == -1)
			printf("To Add error Handler function\n");
			// error_handler("Duplicating write pipe to STDOUT\n", NULL, 1, px);
	}
	else
	{
		if (dup2(px->pipes[i - 1][READ], STDIN_FILENO) == -1)
			printf("To Add error Handler function\n");
			// error_handler("Duplicating read pipe to STDIN", NULL, 1, px);
	}
}

void	redirections_files_setup(int fd, int type, int num_output_fd)
{
	if (fd == -1)
		return ;
	if (type == CHAR_INRED || type == CHAR_HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			printf("To Add error Handler function\n");
			// error_handler("Duplicating read-end pipe to STDOUT", NULL, 1, NULL);
		close(fd);
	}
	else
	{
		if (num_output_fd == 0)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				printf("To Add error Handler function\n");
				// error_handler("Duplicating read-end pipe to STDOUT", NULL, 1, NULL);
		}
		close(fd);
	}
}

void	redirections_setup(t_ast *root)
{
	int	fd;
	int	num_output_fd;

	num_output_fd = 0;
	while (root)
	{
		if (is_redirect_token(root->type))
		{
			fd = open_fd(root->right->content,root->type);
			redirections_files_setup(fd, root->type, num_output_fd);
		}
		if (root->type == CHAR_OUTRED || root->type == CHAR_APPEND)
			num_output_fd++;
		root = root->right;
	}	
}

void	executor_aux(t_px *px, t_ast *root)
{
	if (root == NULL)
		return ;
	if (is_default_token(root->type)/* || is_operator_token(root->type)*/)
	{ 
		printf("[%i] In here in the node with the content: %s\n", px->curr_index, root->content);
		executor(px, px->curr_index, root);
		px->curr_index++;
	}
	else if (is_operator_token(root->type))
	{
		executor_aux(px, root->left);
		executor_aux(px, root->right);
	}
}

int	executor(t_px *px, int i, t_ast *cmd_node)
{
	int	j;

	printf("\n\nEntered in the executor: %i\n\n", i);
	px->pids[i] = fork();
	if (px->pids[i] == -1)
		exit(EXIT_FAILURE);
	if (px->pids[i] == 0)
	{
		if (px->num_pipes !=0)
		{
			child_pipe_setup(px, i);
			j = -1;
			while (++j < px->num_pipes)
			{
				close(px->pipes[j][0]);
				close(px->pipes[j][1]);
			}
		}
		redirections_setup(cmd_node);
		// if (px->argv[i + 2 + px->here_doc][0] == 0)
		// 	error_handler("No command ''", NULL, 1, px);
		if (is_default_token(cmd_node->type))
			exec_command(px, cmd_node);
	}
	return (0);
}

/* TODO - Update the function with minishell structs */
void	exec_command(t_px *px, t_ast *cmd_node)
{
	int		j;
	char	**paths;
	char	*final_path;
	char	**commands;

	paths = path_extractor();
	if (paths == NULL)
		printf("To Add error Handler function\n");
		// error_handler("Error: problem envp file path", NULL, 1);
	commands = commands_extractor(cmd_node);
	j = 0;
	while (paths[j])
	{
		final_path = ft_strjoin_3(paths[j], '/', commands[0]);
		if (access(final_path, F_OK) == 0)
			execve_checker(final_path, commands, paths);
		free (final_path);
		j++;
	}
	if (access(commands[0], F_OK) == 0)
		execve_checker(NULL, commands, paths);
	free_arrays(commands);
	free_arrays(paths);
	free_px(px);
	printf("To Add error Handler function\n");
	// error_handler("command not found", NULL, 127);
}

char	**commands_extractor(t_ast *cmd_node)
{
	t_ast	*temp;	
	int		count;
	char	**commands;

	if (cmd_node == NULL || cmd_node->content == NULL
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
		commands[count] = ft_strdup(temp->content);
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
		printf("To Add error Handler function\n");
		// error_handler("Malloc problem in ft_strjoin_3", NULL, 1, NULL);
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
	int	i;

	if (px->pipes != NULL)
	{
		i = -1;
		while (++i < px->num_pipes)
			free(px->pipes[i]);
		free(px->pipes);
	}
	if (px->pids != NULL)
		free(px->pids);
	free(px);
}

void	execve_checker(char *f_path, char **comms, char **paths)
{
	t_global *global;

	global = global_struct();
	if (execve(f_path, comms, global->ev) == -1 && f_path != NULL)
	{
		free(f_path);
		free_arrays(comms);
		free_arrays(paths);
		printf("To Add error Handler function\n");
		// error_handler("execve call:", NULL, 1, px);
	}
	else if (execve(comms[0], comms, global->ev) == -1 && f_path == NULL)
	{
		free_arrays(comms);
		free_arrays(paths);
		printf("To Add error Handler function\n");
		// error_handler("execve call:", NULL, 1, px);
	}
}

int executor_function(t_ast *root_tree)
{
	int		j;
	int		num;
	t_px	*px;
	int		status;
	
	if (root_tree == NULL)
		return (EXIT_FAILURE);
	px = initialize_px(root_tree);
	printf("px->pids: %p\n", px->pids);
	printf("px->pipes: %p\n", px->pipes);
	printf("px->num_pipes: %i\n", px->num_pipes);
	printf("px->num_commands: %i\n", px->num_commands);
	printf("px->root_tree: %p\n", px->root_tree);

	executor_aux(px, px->root_tree);
	j = -1;
	while (++j < px->num_pipes)
	{
		close(px->pipes[j][READ]);
		close(px->pipes[j][WRITE]);
	}
	num = -1;
	while (++num < px->num_commands)
	{
		printf("%i\n", px->pids[num]);
		waitpid(px->pids[num], &status, 0);
	}
	free_px(px);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_SUCCESS);
}
