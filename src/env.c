#include "../includes/minishell.h"

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
		global->ev[0] = to_add;
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
		new_env[count] = to_add;
		new_env[count + 1] = NULL;
		free(global->ev);
		global->ev = new_env;
	}
}