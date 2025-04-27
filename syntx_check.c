/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntx_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:13:27 by anel-men          #+#    #+#             */
/*   Updated: 2025/04/19 11:25:44 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *input)
{
	int		i;
	int		input_length;
	char	quote_type;
	int		in_quote;

	i = 0;
	input_length = ft_strlen(input);
	in_quote = 0;
	quote_type = '\0';
	while (i < input_length)
	{
		if (!in_quote && (input[i] == '"' || input[i] == '\''))
		{
			in_quote = 1;
			quote_type = input[i];
		}
		else if (in_quote && input[i] == quote_type)
		{
			in_quote = 0;
			quote_type = '\0';
		}
		i++;
	}
	return (check_quotes_helper(in_quote));
}

int	check_quotes_helper(int in_quote)
{
	if (in_quote)
	{
		write(2, "minishell: syntax error near unexpected token 'newline'\n",
			56);
		return (1);
	}
	return (0);
}

int	print_error_pipe(void)
{
	write(2, "minishell:  syntax error near unexpected token `|'\n",
		51);
	return (1);
}

int	error_pipi(t_token *token_list)
{
	t_token	*list;
	int		error_flag;

	error_flag = 0;
	list = token_list;
	while (list)
	{
		if (list == token_list && list->TOKIN
			&& strcmp(list->TOKIN, "pipe_token") == 0)
			error_flag = 1;
		if (list->TOKIN && strcmp(list->TOKIN, "pipe_token") == 0
			&& (!list->next || !list->next->TOKIN
				|| strcmp(list->next->TOKIN, "word_tokin") != 0))
			error_flag = 1;
		if (list->TOKIN && list->next && list->next->TOKIN
			&& strcmp(list->TOKIN, "pipe_token") == 0
			&& strcmp(list->next->TOKIN, "pipe_token") == 0)
			error_flag = 1;
		if (error_flag == 1)
			return (print_error_pipe());
		list = list->next;
	}
	return (0);
}

int	check_syntax_errors(t_token *token_list)
{
	t_token	*current;
	int		error_found;

	error_found = 0;
	current = token_list;
	while (current && !error_found)
	{
		if (strcmp(current->TOKIN, "word_tokin") == 0)
			error_found = check_invalid_filename(current->DATA);
		current = current->next;
	}
	return (error_found);
}
