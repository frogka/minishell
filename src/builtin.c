#include "../includes/minishell.h"

int	builtin_functions(t_ast *node, char **comms, t_px *px, int to_exit)
{
	int	exit_code;

	exit_code = builtin_execution(node);
	if (exit_code == NO_BUILTIN)
		return (0);
	else if(to_exit == TO_EXIT)
	{
		free_arrays(comms);
		free_px(px);
		free_struct_to_free();
		exit(exit_code);
	}		
	else
	{
		free_px(px);
		return (exit_code);
	}
}

int	is_builtin(t_ast *n)
{
	if ((ft_strncmp("echo", n->data, 4) == 0 && ft_strlen(n->data) == 4)
		|| (ft_strncmp("cd", n->data, 2) == 0 && ft_strlen(n->data) == 2)
		|| (ft_strncmp("pwd", n->data, 3) == 0 && ft_strlen(n->data) == 3)
		|| (ft_strncmp("export", n->data, 6) == 0 && ft_strlen(n->data) == 6)
		|| (ft_strncmp("unset", n->data, 5) == 0 && ft_strlen(n->data) == 5)
		|| (ft_strncmp("env", n->data, 3) == 0 && ft_strlen(n->data) == 3)
		|| (ft_strncmp("exit", n->data, 4) == 0 && ft_strlen(n->data) == 4))
		return (1);
	else
		return(0);
}

int	builtin_execution(t_ast *n)
{
	if (ft_strncmp("echo", n->data, 4) == 0 && ft_strlen(n->data) == 4)
		return (echo_builtin(n->left));
	else if (ft_strncmp("cd", n->data, 2) == 0 && ft_strlen(n->data) == 2)
		return (cd_builtin(n->left));
	else if (ft_strncmp("pwd", n->data, 3) == 0 && ft_strlen(n->data) == 3)
		return (pwd_builtin());
	// else if (ft_strncmp("export", n->data, 6) == 0 && ft_strlen(n->data) == 6)
	// 	return (export_builtin());
	// else if (ft_strncmp("unset", n->data, 5) == 0 && ft_strlen(n->data) == 5)
	// 	return (unset_builtin());
	// else if (ft_strncmp("env", n->data, 3) == 0 && ft_strlen(n->data) == 3)
	// 	return (env_builtin());
	// else if (ft_strncmp("exit", n->data, 4) == 0 && ft_strlen(n->data) == 4)
	// 	return (exit_builtin());
	else
		return(NO_BUILTIN);
}

int	pwd_builtin(void)
{
	size_t		size;
	char	buffer[1024];

	size = 1024;
	getcwd(buffer, size);
	printf("%s\n", buffer);
	return (EXIT_SUCCESS);
}

int	echo_builtin(t_ast *node)
{
	int		count;
	t_ast	*initial_node;
	int		option;

	option = 0;
	if (ft_strncmp(node->data, "-n", 2) == 0 && ft_strlen(node->data) == 2)
	{
		node = node->left;
		option = 1;
	}
	initial_node = node;
	count = 0;
	while (node)
	{
		count++;
		node = node->left;
	}
	while (initial_node)
	{
		if (count > 1)
			printf("%s ", initial_node->data);
		else
			printf("%s", initial_node->data);
		initial_node = initial_node->left;
		count--;
	}
	if (option == 0)
		printf("\n");
	return (EXIT_SUCCESS);
}

/* Checks whether env_to_change exists. If yes, change it to new_env. If not, add it to the end.*/
void	update_env(char *env_to_change, char *new_env, char *to_free)
{
	t_global	*global;
	int			count;

	count = -1;
	global = global_struct();
	while (global->ev[++count])
	{
		if (ft_strncmp(env_to_change, global->ev[count], ft_strlen(env_to_change)) == 0)
			break;
	}
	if (global->ev[count])
	{
		free(global->ev[count]);
		global->ev[count] = ft_strjoin(env_to_change, new_env);
	}
	else
		add_env(new_env);
	free(to_free);
}

int	cd_builtin(t_ast *node)
{
	int		count;
	t_ast	*initial_node;
	char	*home;
	char	buffer[1024];

	getcwd(buffer, 1024);
	initial_node = node;
	count = 0;
	while (node)
	{
		count++;
		node = node->left;
	}
	if (count == 0)
	{
		home = find_ev("HOME");
		if (home[0] == 0)
		{
			free(home);
			error_handler("cd: HOME not set", NULL, EXIT_FAILURE, NULL);
		}
		else
		{
			if (chdir(home) == -1)
			{
				free(home);
				error_handler("cd: Path given is not valid", NULL, EXIT_FAILURE, NULL);
			}
			else
			{
				update_env("OLDPWD=", buffer, NULL);
				update_env("PWD=",home, home);
				return (EXIT_SUCCESS);
			}
		}
	}
	else if (count == 1)
	{
		if (chdir(initial_node->data) == -1)
			error_handler("cd: Path given is not valid", NULL, EXIT_FAILURE, NULL);
		else
		{
			update_env("OLDPWD=", buffer, NULL);
			update_env("PWD=", initial_node->data, NULL);
			return (EXIT_SUCCESS);
		}
	}
	else
		error_handler("cd: Too many arguments", NULL, EXIT_FAILURE, NULL);
	return (EXIT_FAILURE);
}