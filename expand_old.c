// #include "minishell.h"

// int	expand_handle_helper0(t_exp_helper *expand)
// {
// 	if (expand->original[expand->i] == '\'')
// 	{
// 		if (expand->quote_state == 0)
// 			expand->quote_state = 1;
// 		else if (expand->quote_state == 1)
// 			expand->quote_state = 0;
// 		expand->expanded[expand->j++] = expand->original[expand->i++];
// 		return (1);
// 	}
// 	else if (expand->original[expand->i] == '"')
// 	{
// 		if (expand->quote_state == 0)
// 			expand->quote_state = 2;
// 		else if (expand->quote_state == 2)
// 			expand->quote_state = 0;
// 		expand->expanded[expand->j++] = expand->original[expand->i++];
// 		return (1);
// 	}
// 	return (0);
// }

// int	helper3(t_exp_helper *expand, int exit_status)
// {
// 	if (expand->original[expand->i] == '?')
// 	{
// 		expand->var_value = ft_itoa(exit_status);
// 		expand->i++;
// 		return (1);
// 	}
// 	return (0);
// }

// int	expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env)
// {
// 	if (expand->original[expand->i] == '$' && expand->quote_state != 1)
// 	{
// 		expand->i++;
// 		if (!helper3(expand, exit_status))
// 		{
// 			expand->start = expand->i;
// 			while (expand->original[expand->i]
// 				&& is_valid_var_char(expand->original[expand->i]))
// 				expand->i++;
// 			expand->var_name = malloc(expand->i - expand->start + 1);
// 			strncpy(expand->var_name, expand->original + expand->start,
// 				expand->i - expand->start);
// 			expand->var_name[expand->i - expand->start] = '\0';
// 			expand->var_value = lookup_variable(expand->var_name, env);
// 			free(expand->var_name);
// 		}
// 		expand->k = 0;
// 		while (expand->var_value && expand->var_value[expand->k])
// 			expand->expanded[expand->j++] = expand->var_value[expand->k++];
// 		return (free(expand->var_value), 1);
// 	}
// 	return (0);
// }

// void	process_token(t_token *tmp, t_exp_helper *expand,
// 					t_env *env, int exit_status)
// {
// 	if (!expand_fill(expand, tmp))
// 		return ;
// 	while (expand->original[expand->i])
// 		if (!expand_handle_helper0(expand)
// 			&& !expand_handle_helper1(expand, exit_status, env))
// 			expand->expanded[expand->j++] = expand->original[expand->i++];
// 	expand->expanded[expand->j] = '\0';
// 	tmp->befor_DATA_expand = ft_strdup(expand->original);
// 	free(tmp->DATA);
// 	tmp->DATA = expand->expanded;
// }

// void	expand_handle(t_token *token_list, t_env *env, int exit_status)
// {
// 	t_token			*tmp;
// 	t_exp_helper	*expand;

// 	expand = (t_exp_helper *)malloc(sizeof(t_exp_helper));
// 	if (!expand)
// 		return ;
// 	tmp = token_list;
// 	while (tmp)
// 	{
// 		if (strcmp(tmp->TOKIN, "word_tokin") == 0)
// 			process_token(tmp, expand, env, exit_status);
// 		tmp = tmp->next;
// 	}
// 	free(expand);
// }
