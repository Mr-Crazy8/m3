#include "minishell.h"

// char	*ft_strncpy(char *dest, const char *src, unsigned int n)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (src[i] != '\0' && i < n)
// 	{
// 		dest[i] = src[i];
// 		i++;
// 	}
// 	while (i < n)
// 	{
// 		dest[i] = '\0';
// 		i++;
// 	}
// 	return (dest);
// }

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	size_t	s_len;
// 	char	*substr;

// 	if (s == NULL)
// 	{
// 		return (NULL);
// 	}
// 	s_len = ft_strlen(s);
// 	if (start >= s_len)
// 		return (ft_strdup(""));
// 	if (len > s_len - start)
// 		len = s_len - start;
// 	substr = (char *)malloc((len + 1) * sizeof(char));
// 	if (substr == NULL)
// 		return (NULL);
// 	ft_strncpy(substr, s + start, len);
// 	substr[len] = '\0';
// 	return (substr);
// }



// int was_delimiter_quoted(char *orig_token) ============>
// {
//     // Check if original delimiter had quotes
//     if (orig_token && 
//         ((orig_token[0] == '\'' && orig_token[strlen(orig_token)-1] == '\'') ||
//          (orig_token[0] == '\"' && orig_token[strlen(orig_token)-1] == '\"')))
//         return 1;
//     return 0;
// }


//for this func my logic it to count fo $ and if  is even (doller % 2 == 0)
//i will just remove the quotes with selective_remove_quotes
//else i will use sudstr func to creat new string from tab 1 not 0 so 
//if we have $$$ the new string will have $$
// char *check_for_doller(char *orig_token) ================>
// {
//     int doller = 0;
//     while (orig_token[doller] != '\0' && orig_token[doller] == '$')
//         doller++;

//     if (orig_token[doller] != '\0' && (orig_token[doller] == '\'' || orig_token[doller] == '\"'))
//     {
//         if (doller % 2 == 0)
//            return selective_remove_quotes(orig_token, 1);
//         else {
//             // For odd number of dollars, remove one dollar and then process
//             char *substr = ft_substr(orig_token, 1, strlen(orig_token) - 1);
//             char *result = selective_remove_quotes(substr, 1);
//             free(substr);
//             return result;
//         }
//     }
//     return orig_token;
// }



// char *heredoc_delemter(char *orig_token)
// {
//     char *new_delemter;

//     if (!orig_token)
//         return NULL;
//     if (was_delimiter_quoted(orig_token) == 0)
//     {
//         if (orig_token[0] == '$')
//         {
//             new_delemter = check_for_doller(orig_token);
//         }
//         else 
//         {
//             new_delemter = selective_remove_quotes(orig_token, 1);
//         }
        
//         return new_delemter;
//     }
//     return ft_strdup(orig_token);
// }


// int check_for_quotes(char *str) ================>
// {
//     int i = 0;
//     int count_signal = 0;
//     int count_duble = 0;
//     while (str[i])
//     {
//         if (str[i] == '\'')
//             count_signal++;
//         else if (str[i] == '\"')
//             count_duble++;
//         i++;
//     }
//     if (count_signal != 0 || count_duble != 0)
//         return 1;
//     return 0;
// }


// char *heredoc_delemter(char *orig_token) -===============>
// {
//     char *new_delemter;

//     if (!orig_token)
//         return NULL;
    
//     if (was_delimiter_quoted(orig_token) == 1)  // If quoted
//     {
//         // Simply remove the quotes
//         new_delemter = selective_remove_quotes(orig_token, 1);  // Don't free original
//         return new_delemter;
//     }
    
//     // Handle $ in unquoted delimiters
//     if (orig_token[0] == '$')
//     {
//         new_delemter = check_for_doller(orig_token);
//         if (check_for_quotes(new_delemter) == 1)
//         {
//                 new_delemter = selective_remove_quotes(orig_token, 1);  // Don't free original
//                 return new_delemter;
//          }
//         return new_delemter;
//     }
//     // else if (check_for_quotes(orig_token) == 1)
//     // {
//     //     new_delemter = selective_remove_quotes(orig_token, 1);  // Don't free original
//     //     return new_delemter;
//     // }
    
//     return ft_strdup(orig_token);
// }


// char *process_heredoc_epxand(char *line, t_env *env, int exit_status, char *orig_delimiter) ===========================>
// {
//     t_exp_helper *expand;
//     char *result;
//     if (was_delimiter_quoted(orig_delimiter) || check_for_quotes(orig_delimiter))
//         return ft_strdup(line);
//     expand = malloc(sizeof(t_exp_helper));
//     if (!expand)
//         return NULL;
//     process_string(line, expand, env, exit_status);
//     result = expand->expanded;
//     free(expand);

//     printf("ep =========> %s\n", result);
//     return result;
// }


// char *heredoc(char *delmeter, t_env *env, int exit_status, char *orig_delimiter) ===================+>
// {
//     char *line;
//     char *heredoc;
//     char *tmp1;
//     char *tmp2;
//     char *processed_delimiter = heredoc_delemter(delmeter);

//     heredoc = NULL;

//      printf("delimiter : ==============> %s\n", delmeter);
//      printf("processed_delimiter : ==============> %s\n", processed_delimiter);
//     while(1)
//     {
//        line = readline("> ");
//        if (!line)
//        {
//             write(1, "\n", 1);
//             free(processed_delimiter);
//             return heredoc;
//        }
//        if (strcmp(line, processed_delimiter) == 0)
//        {
//         free(line);
//         break;
//        }
//        tmp1 = ft_strjoin(process_heredoc_epxand(line, env, exit_status, orig_delimiter), "\n");
//        free(line);
//        if (!tmp1)
//        {
//         free(processed_delimiter);
//         return heredoc;
//        }
//         if (heredoc == NULL)
//             {
//                 heredoc = tmp1;
//             }
//         else 
//         {
//             tmp2 = ft_strjoin(heredoc, tmp1);
//             free(heredoc);
//             free(tmp1);

//             if (!tmp2)
//             {
//                 free(processed_delimiter);
//                 return NULL;
//             }
//             heredoc = tmp2;
//         }
//     }
//     free(processed_delimiter);
//     return heredoc;
// }


// void heredoc_delemter(t_cmd *cmd_list)
// {
//     t_cmd *tmp = cmd_list;
//     char *new_delemter;
//     while (tmp)
//     {
//         t_redir *redir = tmp->redirs;
//         while (redir)
//         {
//             if (redir->type == 3) // Heredoc
//             {
//                 if (!was_delimiter_quoted(redir->orig_token))
//                 {
//                     if (redir->orig_token[0] == '$')
//                     {
//                         new_delemter = check_for_doller(redir->orig_token);
//                         free(redir->orig_token);
//                         redir->orig_token = new_delemter;
//                     }
//                     else 
//                     {
//                         new_delemter = selective_remove_quotes(redir->orig_token, 1);
//                         free(redir->orig_token);
//                         redir->orig_token = new_delemter;
//                     }
//                 }
//             }
//             redir = redir->next;
//         }
//         tmp = tmp->next;
//     }
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <readline/readline.h>
// #include <readline/history.h>

// // Function to extract delimiter from heredoc syntax
// char *extract_delimiter(char *cmd)
// {
//     char *delimiter = NULL;
//     char *start = strstr(cmd, "<<");
    
//     if (!start)
//         return NULL;
    
//     // Skip the << and any spaces
//     start += 2;
//     while (*start && (*start == ' ' || *start == '\t'))
//         start++;
    
//     if (!*start)
//         return NULL;
    
//     // Create a copy of the whole delimiter
//     return strdup(start);
// }

// int main(int argc, char *argv[], char *env[])
// {
//     char *input;
//     char *delimiter;
//     char *content;
//     t_env *env_struc = NULL;  // Use your existing env or NULL
//     int exit_status = 42;
//     int running = 1;
//     env_maker(env, &env_struc);
//     printf("\033[1;33m===== Heredoc Shell Simulator (%s) =====\033[0m\n", "2025-05-08");
//     printf("Current User: %s\n\n", "Mr-Crazy8");
//     printf("Type commands like: << EOF or << 'EOF' (with quotes)\n");
//     printf("Type 'exit' to quit\n\n");
    
//     // You can initialize env here with your existing code if needed
    
//     while (running)
//     {
//         input = readline("heredoc-sim$ ");
//         if (!input)
//             break;
        
//         if (strlen(input) > 0)
//             add_history(input);
        
//         if (strcmp(input, "exit") == 0)
//         {
//             free(input);
//             running = 0;
//             continue;
//         }
        
//         if (strncmp(input, "<<", 2) == 0)
//         {
//             // Extract delimiter from command
//             delimiter = extract_delimiter(input);
//             printf("delimiter : ==============> %s\n", delimiter);
//             if (delimiter)
//             {
//                 // Call your heredoc function
//                 content = heredoc(delimiter, env_struc, exit_status, delimiter);
                
//                 // Display the content
//                 printf("\n--- Heredoc Content ---\n");
//                 printf("%s", content ? content : "(null)");
//                 // Make sure output ends with a newline
//                 if (content && content[0] != '\0' && content[strlen(content)-1] != '\n')
//                     printf("\n");
//                 printf("--- End of Content ---\n\n");
                
//                 // Free memory
//                 free(delimiter);
//                 if (content)
//                     free(content);
//             }
//             else
//             {
//                 printf("Error: Invalid heredoc syntax\n");
//             }
//         }
//         else if (strlen(input) > 0)
//         {
//             printf("Unknown command. Use << DELIMITER to start a heredoc.\n");
//         }
        
//         free(input);
//     }
    
//     printf("\nExiting heredoc simulator.\n");
//     // Free your environment if needed using your existing code
    
//     return 0;
// }


