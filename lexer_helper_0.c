/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:04:29 by anel-men          #+#    #+#             */
/*   Updated: 2025/05/09 19:06:09 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	int		i;
	int		j;
	int		s_len;
	char	*dup;

	i = 0;
	j = 0;
	s_len = ft_strlen(s);
	dup = (char *)malloc(s_len + 1);
	if (dup == NULL)
	{
		return (NULL);
	}
	while (s[i] != '\0')
	{
		dup[j] = s[i];
		j++;
		i++;
	}
	dup[j] = '\0';
	return (dup);
}



t_token	*creat_token(char *data, char *token_type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->DATA = ft_strdup(data);
	if (!(new_token->DATA))
		return (free(new_token), NULL);
	new_token->TOKIN = ft_strdup(token_type);
	if (!(new_token->TOKIN))
	{
		free(new_token->DATA);
		free(new_token);
		return (NULL);
	}
	new_token->befor_DATA_expand = NULL;
	new_token->next = NULL;
	return (new_token);
}

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
