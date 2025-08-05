#include "../includes/minishell.h"

/* TODO:
==> Check If the export function is blocking variable names that should not be allowed */

int	builtin_functions(t_ast *node, char **comms, t_px *px, int to_exit)
{
	int	exit_code;
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	exit_code = builtin_execution(node);
	if (exit_code == NO_BUILTIN)
		return (0);
	else if(to_exit == TO_EXIT)
	{
		free_arrays(comms);
		free_px(px);
		free_struct_to_free();
		free_global_struct();
		rl_clear_history();
		free(pl->prompt);
		exit(exit_code);
	}		
	else
		return (exit_code);
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
	else if (ft_strncmp("export", n->data, 6) == 0 && ft_strlen(n->data) == 6)
		return (export_builtin(n->left));
	else if (ft_strncmp("unset", n->data, 5) == 0 && ft_strlen(n->data) == 5)
		return (unset_builtin(n->left));
	else if (ft_strncmp("env", n->data, 3) == 0 && ft_strlen(n->data) == 3)
		return (env_builtin(n->left));
	else if (ft_strncmp("exit", n->data, 4) == 0 && ft_strlen(n->data) == 4)
		return (exit_builtin());
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
	if (node == NULL)
	{
		printf("\n");
		return (EXIT_SUCCESS);
	}	
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
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		else
		{
			if (chdir(home) == -1)
			{
				free(home);
				ft_putstr_fd("cd: No such file or directory\n", STDERR_FILENO);
				return (EXIT_FAILURE);
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
		{
			ft_putstr_fd("cd: No such file or directory\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		else
		{
			update_env("OLDPWD=", buffer, NULL);
			update_env("PWD=", initial_node->data, NULL);
			return (EXIT_SUCCESS);
		}
	}
	ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}

t_prompt_line	*to_prompt_line_struct(void)
{
	static t_prompt_line	pl;

	return (&pl);
}

int	exit_builtin(void)
{
	t_prompt_line	*pl;

	pl = to_prompt_line_struct();
	free(pl->prompt);
	exit(EXIT_SUCCESS);
}

void	print_export_builtin(void)
{
	t_global	*global;
	int			i;

	global = global_struct();
	i = -1;
	if (global->ev == NULL)
		return;
	while (global->ev[++i])
		printf("declare -x %s\n", global->ev[i]);
}

void	print_env_builtin(void)
{
	t_global	*global;
	int			i;

	global = global_struct();
	i = 0;
	if (global->ev == NULL)
		return;
	while (global->ev[i])
	{
		if (ft_strnstr(global->ev[i], "=", ft_strlen(global->ev[i])) != NULL)
			printf("%s\n", global->ev[i]);
		i++;
	}
}

int	check_valid_export_aux(char *str)
{
	int	i;

	if (ft_isdigit(str[0]))
		return (EXIT_FAILURE);
	i = -1;
	while (str[++i])
	{
		if(!ft_isalnum(str[i]) && str[i] != '_')
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_valid_export(char *str)
{
	char	*es_pos;
	char	*env_to_change;

	if (str[0] == '=')
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd("=", STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	es_pos = ft_strchr(str, '='); 
	if (es_pos != NULL)
		env_to_change = ft_substr(str, 0, es_pos - str);
	else
		env_to_change = str;
	if (check_valid_export_aux(env_to_change) == EXIT_FAILURE)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(env_to_change, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		if (es_pos != NULL)
			free(env_to_change);		
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	export_builtin(t_ast *node)
{
	char	*es_pos;
	char	*env_to_change;
	char	*new_env;

	if (node == NULL)
		print_export_builtin();
	while (node)
	{
		if (check_valid_export(node->data) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		es_pos = ft_strchr(node->data, '='); 
		if (es_pos != NULL)
		{
			env_to_change = ft_substr(node->data, 0, es_pos - node->data);
			new_env = ft_substr(node->data, es_pos - node->data, ft_strlen(node->data) - (es_pos - node->data));
			update_env(env_to_change, new_env, env_to_change);
			free(new_env);
		}
		else
			update_env(node->data, "", NULL);
		node = node->left;
	}
	return (EXIT_SUCCESS);
}

int	unset_builtin(t_ast *node)
{
	if (node == NULL)
		return (EXIT_SUCCESS);
	while (node)
	{
		remove_env(node->data);
		node = node->left;
	}
	return (EXIT_SUCCESS);
}

int	env_builtin(t_ast *node)
{
	if (node == NULL)
	{
		print_env_builtin();
		return (EXIT_SUCCESS);
	}
	else
	{
		ft_putstr_fd("env: no options or arguments allowed\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
}
