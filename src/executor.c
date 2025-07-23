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
	// printf("This is the type: '%c'\n", root_tree->type);
	if (root_tree == NULL)
		return (0);
	if (is_default_token(root_tree->type))
	{
		total++;
		// printf("This is the content: %s. Total :%i\n", root_tree->content, total);
	}
	else if (is_operator_token(root_tree->type))
	{
		// printf("Inside the second if. This is the content: %s. Total :%i\n", root_tree->content, total);
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
	printf("px->pipes: %p\n", px->root_tree);
	return (EXIT_SUCCESS);
}
