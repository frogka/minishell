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
/*
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

int	executor_aux(t_px *px)
{
	int	i;

	i = 0;
	if (px->root_tree == NULL)
		return (0);
	if (is_default_token(px->root_tree->type))
	{
		executor(px, i);
		i++;
	}
	else if (is_operator_token(px->root_tree->type))
	{
		i += count_number_commands(px->root_tree->left);
		i += count_number_commands(px->root_tree->right);
	}
	return (i);
}

void	child_pipe_setup(t_px *px, int i)
{
	if (i == 0)
	{
		if (px->here_doc == 0 && dup2(px->fd_input, STDIN_FILENO) == -1)
			error_handler("Duplicating read pipe to STDIN", NULL, 1, px);
		if (dup2(px->pipes[0][WRITE], STDOUT_FILENO) == -1)
			error_handler("Duplicating write pipe to STDOUT\n", NULL, 1, px);
	}
	else if (i < px->num_pipes)
	{
		if (dup2(px->pipes[i - 1][READ], STDIN_FILENO) == -1)
			error_handler("Duplicating read pipe to STDIN", NULL, 1, px);
		if (dup2(px->pipes[i][WRITE], STDOUT_FILENO) == -1)
			error_handler("Duplicating write pipe to STDOUT\n", NULL, 1, px);
	}
	else
	{
		if (dup2(px->pipes[i - 1][READ], STDIN_FILENO) == -1)
			error_handler("Duplicating read pipe to STDIN", NULL, 1, px);
		if (dup2(px->fd_output, STDOUT_FILENO) == -1)
			error_handler("Duplicating write pipe to STDOUT\n", NULL, 1, px);
	}
}

int	executor(t_px *px, int i)
{
	int	j;

	px->pids[i] = fork();
	if (px->pids[i] == -1)
		exit(EXIT_FAILURE);
	if (px->pids[i] == 0)
	{
		child_pipe_setup(px, i);
		j = -1;
		while (++j < px->num_pipes)
		{
			close(px->pipes[j][0]);
			close(px->pipes[j][1]);
		}
		// if (px->argv[i + 2 + px->here_doc][0] == 0)
		// 	error_handler("No command ''", NULL, 1, px);
		exec_command(px, i);
	}
	return (0);
}*/

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

	return (EXIT_SUCCESS);
}
