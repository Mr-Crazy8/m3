/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:53:47 by anel-men          #+#    #+#             */
/*   Updated: 2025/04/19 11:53:56 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if (dest == src)
		return (dest);
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}



static size_t	count_words(char const *s, char c)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			n++;
		i++;
	}
	return (n);
}

static char	**split_string(char const *s, char c, char **split, size_t n)
{
	size_t	i;
	size_t	j;
	size_t	start;
	size_t	len;

	i = 0;
	j = 0;
	while (s[i] && j < n)
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		len = i - start;
		split[j] = (char *)malloc(len + 1);
		if (!split[j])
			return (free_split(split, j));
		ft_memcpy(split[j], &s[start], len);
		split[j][len] = '\0';
		j++;
	}
	split[j] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	size_t	n;
	char	**split;

	if (s == NULL)
		return (NULL);
	n = count_words(s, c);
	split = (char **)malloc((n + 1) * sizeof(char *));
	if (split == NULL)
		return (NULL);
	return (split_string(s, c, split, n));
}
