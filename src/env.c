#include "../includes/minishell.h"



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