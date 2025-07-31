#include "../includes/minishell.h"

/* Checks whether env_to_change exists. If yes, change it to new_env. If not, add it to the end.*/
void	update_env(char *env_to_change, char *new_env, char *to_free)
{
	t_global	*global;
	char		*to_add;
	int			count;

	count = -1;
	global = global_struct();
	if(global->ev == NULL)
	{
		to_add = ft_strjoin(env_to_change, new_env);
		add_env(to_add);
		free(to_add);
	}
	else
	{
		while (global->ev[++count])
		{
			if (ft_strncmp(env_to_change, global->ev[count], ft_strlen(env_to_change)) == 0
					&& (global->ev[count][ft_strlen(env_to_change)] == 0
					|| global->ev[count][ft_strlen(env_to_change)] == '='))
				break;
		}
		if (global->ev[count])
		{
			if (ft_strlen(new_env) != 0)
			{
				free(global->ev[count]);
				global->ev[count] = ft_strjoin(env_to_change, new_env);
			}
		}
		else
		{
			to_add = ft_strjoin(env_to_change, new_env);
			add_env(to_add);
			free(to_add);
		}
	}
	free(to_free);
}

void	remove_env_aux(int count)
{
	t_global	*global;
	char		**new_env;
	int			i;
	int			j;

	global = global_struct();
	i = -1;
	while (global->ev[++i]);
	if (i == 1)
	{
		free(global->ev[0]);
		free(global->ev);
		global->ev = NULL;
		return ;
	}
	new_env = malloc(sizeof(char *) * i);
	i = -1;
	j = 0;
	while (global->ev[++i])
	{
		if (i != count)
			new_env[j++] = global->ev[i];
		else
			free(global->ev[i]);
	}
	new_env[i - 1] = NULL;
	free(global->ev);
	global->ev = new_env;
}

void	remove_env(char *env_to_remove)
{
	t_global	*global;
	int			count;

	count = -1;
	global = global_struct();
	if (global->ev == NULL)
		return ;
	while (global->ev[++count])
	{
		if (ft_strncmp(env_to_remove, global->ev[count], ft_strlen(env_to_remove)) == 0)
			break;
	}
	if (global->ev[count])
		remove_env_aux(count);
}

void	add_env(char *to_add)
{
	t_global	*global;
	char		**new_env;
	int			count;
	int			i;

	global = global_struct();
	if(global->ev == NULL)
	{
		global->ev = malloc(sizeof(char*) * (2));
		global->ev[0] = ft_strdup(to_add);
		global->ev[1] = NULL;
		return ;
	}
	else
	{
		count = 0;
		while (global->ev[count])
			count++;
		new_env = malloc(sizeof(char*) * (count + 2));
		i = -1;
		while (++i < count)
			new_env[i] = global->ev[i];
		new_env[count] = ft_strdup(to_add);
		new_env[count + 1] = NULL;
		free(global->ev);
		global->ev = new_env;
	}
}

char	*find_ev(char *to_expand)
{
	t_global 	*global;
	int			i;
	char		*result;

	global = global_struct();
	i = -1;
	while (global->ev[++i])
	{
		if (ft_strncmp(to_expand, global->ev[i], ft_strlen(to_expand)) == 0
				&& (global->ev[i][ft_strlen(to_expand)]) == '=')
		{
			result = ft_substr(global->ev[i], ft_strlen(to_expand) + 1,
						ft_strlen(global->ev[i]));
			return (result);
		}
	}
	return (ft_strdup(""));
}
