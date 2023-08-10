/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeok <jinhyeok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 20:40:00 by jinhyeok          #+#    #+#             */
/*   Updated: 2023/08/07 20:40:24 by jinhyeok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_unset(t_mini *data, t_env *env)
{
	int			i;
	char		*key;
	t_env_node	*temp;
	t_env_node	*new;

	i = 0;
	if ((data->cmd_size - data->delete) > 1)
	{
		while (++i < data->cmd_size)
		{
			if (data->command[i])
			{
				if (ft_getexport(data->command[i], env))
				{
					key = data->command[i];
					new = realloc_evnode(key, NULL, env->export, -1);
					temp = env->export;
					env->export = new;
					env_free(temp);
				}
				if (ft_getenv(key, env))
				{
					new = realloc_evnode(key, NULL, env->node, -1);
					temp = env->node;
					env->node = new;
					env_free(temp);
				}
			}
		}
	}
}