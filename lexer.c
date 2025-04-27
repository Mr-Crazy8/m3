/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 11:00:55 by anel-men          #+#    #+#             */
/*   Updated: 2025/04/20 18:03:16 by anel-men         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




// Add this function to track the quote state
int handle_quotes(char c, int *quote_state)
{
    if (*quote_state == 0 && (c == '\'' || c == '\"'))
    {
        if (c == '\'')
            *quote_state = 1;
        else
            *quote_state = 2;
        return (1);
    }
    else if ((*quote_state == 1 && c == '\'') || (*quote_state == 2 && c == '\"'))
    {
        *quote_state = 0;
        return (1);
    }
    return (0);
}

// Updated function to check for special characters, respecting quotes
int special_character_cheker_with_quotes(char character, int quote_state)
{
    if (quote_state == 0 && character == '|')
        return (1);
    return (0);
}

void scan_for_token(char *input, int *i, int input_length, int *start_index)
{
    int quote_state;

    quote_state = 0;
    while (*i < input_length && input[*i] == ' ')
        (*i)++;
        
    *start_index = *i;
    
    // Continue until we find a special character that's not in quotes
    while (*i < input_length)
    {
        // Handle quotes - update quote state
        if (input[*i] == '\'' || input[*i] == '\"')
        {
            handle_quotes(input[*i], &quote_state);
            (*i)++;
            continue;
        }
        
        // Check for special characters only if not in quotes
        if (quote_state == 0 && special_character_cheker(input[*i]))
            break;
            
        (*i)++;
    }
}

void handle_block(char *input, int *i, int input_length, t_token **token_list)
{
    int start_index;
    t_token *node;
    char *word;
    
    scan_for_token(input, i, input_length, &start_index);
    
    if (start_index < *i)
    {
        word = ft_substr(input, start_index, *i - start_index);
        if (word)
        {
            node = creat_token(word, "word_tokin");
            free(word);
            if (node)
                ft_lstadd_back_token(token_list, node);
        }
    }
    
    if (*i < input_length && special_character_cheker(input[*i]))
        handle_block_helper(input, i, token_list);
}


t_token	*tokin_list_maker(char *input)
{
	int		i;
	char	current_char;
	int		input_length;
	t_token	*token_list;
	t_token	*node;

	token_list = NULL;
	node = NULL;
	i = 0;
	input_length = ft_strlen(input);
	while (i < input_length)
		handle_block(input, &i, input_length, &token_list);
	return (token_list);
}


