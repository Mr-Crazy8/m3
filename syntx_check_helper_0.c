/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntx_check_helper_0.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:26:07 by anel-men          #+#    #+#             */
/*   Updated: 2025/04/19 11:47:15 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_for_redirection(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (str != NULL && strcmp(str, "|") != 0)
	{
		while (str[i])
		{
			redirection_helper(str, &i, &count);
			if (str[i] == '>' || str[i] == '<')
			{
				count++;
				i++;
			}
			else
				i++;
		}
	}
	return (count);
}

void	check_missing_filename(char *str)
{
	int		i;
	char	operator_type;

	i = 0;
	if (str != NULL && check_for_redirection(str) != 0)
	{
		while (str[i] && str[i] != '>' && str[i] != '<')
			i++;
		if (str[i] == '>' || str[i] == '<')
		{
			operator_type = str[i];
			i++;
			if (str[i] == operator_type)
				i++;
			while (str[i] && str[i] == ' ')
				i++;
			if (str[i] == '\0' || str[i] == '>' || str[i] == '<')
				write(2,
					"minishell:  syntax error \
						near unexpected token `newline'\n",
					58);
		}
	}
}

void	print_error(char *str, int *i)
{
	if (str[(*i)] == '\0')
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
	else if (str[(*i)] == '|')
		write(2, "minishell: syntax error near unexpected token `|'\n",
			51);
	else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
		write(2, "minishell: syntax error near unexpected token `>>'\n",
			52);
	else if (str[(*i)] == '>')
		write(2, "minishell: syntax error near unexpected token `>'\n",
			51);
	else if (str[(*i)] == '<' && str[(*i) + 1] == '<')
		write(2, "minishell: syntax error near unexpected token `<<'\n",
			52);
	else if (str[(*i)] == '<')
		write(2, "minishell: syntax error near unexpected token `<'\n",
			51);
}

int	invalid_filename_checker(char *str, int *i)
{
	char	operator_type;

	operator_type = str[(*i)];
	(*i)++;
	if (str[(*i)] == operator_type)
		(*i)++;
	while (str[(*i)] && str[(*i)] == ' ')
		(*i)++;
	if (str[(*i)] == '\0' || str[(*i)] == '>'
		|| str[(*i)] == '<' || str[(*i)] == '|')
	{
		print_error(str, i);
		return (1);
	}
	else
	{
		while (str[(*i)] && str[(*i)] != ' ' && str[(*i)] != '>'
			&& str[(*i)] != '<' && str[(*i)] != '|')
			(*i)++;
	}
	return (0);
}

int	check_invalid_filename(char *str)
{
	int	i;
	int	found_error;

	i = 0;
	found_error = 0;
	if (str != NULL && check_for_redirection(str) != 0)
	{
		while (str[i] && !found_error)
		{
			while (str[i] && str[i] != '>' && str[i] != '<')
				i++;
			if (!str[i])
				break ;
			if (str[i] == '>' || str[i] == '<')
				found_error = invalid_filename_checker(str, &i);
		}
	}
	return (found_error);
}
