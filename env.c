/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:48:53 by anel-men          #+#    #+#             */
/*   Updated: 2025/04/19 12:01:09 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



t_env	*env_node_maker(char *env[])
{
	t_env	*env_node;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	if (!env || !env[0])
		env_node->key = NULL;
	else
		env_node->key = ft_strdup(env[0]);
	if (!env || !env[1])
		env_node->value = NULL;
	else
		env_node->value = ft_strdup(env[1]);
	env_node->next = NULL;
	return (env_node);
}

void	env_maker(char *env[], t_env **env_struct)
{
	t_env	*tmp;
	t_env	*env_list;
	int		i;
	int		j;
	char	**split;

	env_list = NULL;
	i = 0;
	j = -1;
	while (env[i])
	{
		j = -1;
		split = ft_split(env[i], '=');
		if (split)
		{
			ft_lstadd_back_env(&env_list, env_node_maker(split));
			while (split[++j])
				free((split[j]));
			free(split);
		}
		i++;
	}
	*env_struct = env_list;
}



