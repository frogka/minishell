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
	px->pids = malloc(sizeof(pid_t) * px->num_commands);
	// malloc_error_handler(px->pids, EXIT_FAILURE);
	// create_pipeline(px);
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

int	executor_aux(t_px *px, t_ast *root_tree)
{
	int		i;

	i = 0;
	if (root_tree == NULL)
		return (0);
	if (is_default_token(root_tree->type))
	{
		printf("In here\n");
		executor(px, i, root_tree);
		i++;
	}
	else if (is_operator_token(root_tree->type))
	{
		i += executor_aux(px, root_tree->left);
		i += executor_aux(px, root_tree->right);
	}
	return (i);
}

int	executor(t_px *px, int i, t_ast *cmd_node)
{
	int	j;

	px->pids[i] = fork();
	if (px->pids[i] == -1)
		exit(EXIT_FAILURE);
	if (px->pids[i] == 0)
	{
		printf("Before\n");
		if (px->num_pipes !=0)
		{
			child_pipe_setup(px, i);
			printf("After\n");
			j = -1;
			while (++j < px->num_pipes)
			{
				close(px->pipes[j][0]);
				close(px->pipes[j][1]);
			}
		}
		printf("After\n");
		// if (px->argv[i + 2 + px->here_doc][0] == 0)
		// 	error_handler("No command ''", NULL, 1, px);
		printf("This is the command: %s; %i\n", cmd_node->content, i);
		// exec_command(px, i);
	}
	return (0);
}

/* TODO - Update the function with minishell structs */
int	exec_command(t_px *px, int i)
{
	int		j;
	char	**paths;
	char	*final_path;
	char	**commands;

	paths = path_extractor(px->envp);
	if (paths == NULL)
		error_handler("Error: problem envp file path", NULL, 1);
	commands = ft_split(px->argv[i + 2 + px->here_doc], ' ');
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
	free_arrays(commands);
	free_arrays(paths);
	free_px(px);
	error_handler("command not found", NULL, 127);
	return (0);
}

int executor_function(t_ast *root_tree)
{
	t_px	*px;
	
	if (root_tree == NULL)
		return (EXIT_FAILURE);
	px = initialize_px(root_tree);
	printf("px->pids: %p\n", px->pids);
	printf("px->pipes: %p\n", px->pipes);
	printf("px->num_pipes: %i\n", px->num_pipes);
	printf("px->num_commands: %i\n", px->num_commands);
	printf("px->root_tree: %p\n", px->root_tree);

	executor_aux(px, px->root_tree);
	return (EXIT_SUCCESS);
}
