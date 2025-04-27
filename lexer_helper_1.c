/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:07:47 by anel-men          #+#    #+#             */
/*   Updated: 2025/04/19 11:12:27 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	char	*substr;

	if (s == NULL)
	{
		return (NULL);
	}
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	substr = (char *)malloc((len + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	ft_strncpy(substr, s + start, len);
	substr[len] = '\0';
	return (substr);
}

int	special_character_cheker(char character)
{
	if (character == '|')
		return (1);
	return (0);
}

t_token	*special_character_handle(char *input, int *i)
{
	t_token	*node;
	char	buffer[100];
	int		buffer_index;

	buffer_index = 0;
	node = NULL;
	if (input[(*i)] == ' ')
		(*i)++;
	if (input[(*i)] == '|' )
	{
		node = creat_token("|", "pipe_token");
		(*i)++;
	}
	return (node);
}

void	handle_block_helper(char *input, int *i, t_token **token_list)
{
	t_token	*node;

	node = special_character_handle(input, i);
	if (node)
		ft_lstadd_back_token(token_list, node);
}
