#include "minishell.h"

char	**free_split(char **split, size_t j)
{
	while (j > 0)
		free(split[--j]);
	free(split);
	return (NULL);
}

void free_env_struct(t_env *env_struct)
{
	 t_env *tmp;
	t_env *tmp2;

	tmp = env_struct;       
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
		tmp = tmp2;
	}  
}

void	free_token_list(t_token *token_list)
{
	t_token	*current;
	t_token	*next;

	current = token_list;
	while (current)
	{
		next = current->next;
		free(current->DATA);
		free(current->TOKIN);
		free(current);
		current = next;
	}
}

char **free_split_q(char **split, size_t j)
{
    while (j > 0)
        free(split[--j]);
    free(split);
    return (NULL);
}


void free_split_str(char **split_str)
{
    int j;

    j = 0;
    while (split_str[j])
        free(split_str[j++]);
    free(split_str);
}
