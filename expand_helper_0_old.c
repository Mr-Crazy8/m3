// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   expand_helper_0.c                                  :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: anel-men <anel-men@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/04/21 11:21:05 by anel-men          #+#    #+#             */
// /*   Updated: 2025/04/25 09:55:30 by anel-men         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// #include "minishell.h"

// int process_quote_char(char c, int *quote_state, char *new_str, int *j, int remove_mode)
// {
//     if (c == '\'')
//     {
//         if (*quote_state == 0)
//             *quote_state = 1;
//         else if (*quote_state == 1)
//             *quote_state = 0;
//         if (remove_mode == 0)
//             new_str[(*j)++] = c;
            
//         return (1);
//     }
//     else if (c == '\"')
//     {
//         if (*quote_state == 0)
//             *quote_state = 2;
//         else if (*quote_state == 2)
//             *quote_state = 0;
//         if (remove_mode == 0)
//             new_str[(*j)++] = c;
//         return (1);
//     }
//     return (0);
// }

// char *selective_remove_quotes(char *str, int remove_mode)
// {
//     char *new_str;
//     int i;
//     int j;
//     int quote_state;
    
//     i = 0;
//     j = 0;
//     quote_state = 0;
    
//     if (!str)
//         return (NULL);
        
//     new_str = malloc(strlen(str) + 1);
//     if (!new_str)
//         return (NULL);
        
//     while (str[i])
//     {
//         if (!process_quote_char(str[i], &quote_state, new_str, &j, remove_mode))
//             new_str[j++] = str[i];
//         i++;
//     }
    
//     new_str[j] = '\0';
//     return (new_str);
// }

// void process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode)
// {
//     t_cmd *current;
//     char *processed;
//     int i;

//     current = cmd_list;
//     while (current)
//     {
//         // Process command arguments
//         if (current->args)
//         {
//             i = 0;
//             while (current->args[i])
//             {
//                 processed = selective_remove_quotes(current->args[i], remove_mode);
//                 if (processed)
//                 {
//                     free(current->args[i]);
//                     current->args[i] = processed;
//                 }
//                 i++;
//             }
//         }

//         if (current->cmd)
//         {
//             i = 0;

//                 processed = selective_remove_quotes(current->cmd, remove_mode);
//                 if (processed)
//                 {
//                     free(current->cmd);
//                     current->cmd = processed;
//                 }
//         }
        
//         // Process redirection filenames if any
//         t_redir *redir = current->redirs;
//         while (redir)
//         {
//             if (redir->file)
//             {
//                 processed = selective_remove_quotes(redir->file, remove_mode);
//                 if (processed)
//                 {
//                     free(redir->file);
//                     redir->file = processed;
//                 }
//             }
//             redir = redir->next;
//         }
        
//         // Update cmd pointer if it's pointing to args[0]
//         if (current->args && current->args[0] && current->cmd == current->args[0])
//             current->cmd = current->args[0];
            
//         current = current->next;
//     }
// }