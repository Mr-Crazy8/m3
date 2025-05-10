#include "minishell.h"

int	expand_handle_helper0(t_exp_helper *expand)
{
	if (expand->original[expand->i] == '\'')
	{
		if (expand->quote_state == 0)
			expand->quote_state = 1;
		else if (expand->quote_state == 1)
			expand->quote_state = 0;
		expand->expanded[expand->j++] = expand->original[expand->i++];
		return (1);
	}
	else if (expand->original[expand->i] == '"')
	{
		if (expand->quote_state == 0)
			expand->quote_state = 2;
		else if (expand->quote_state == 2)
			expand->quote_state = 0;
		expand->expanded[expand->j++] = expand->original[expand->i++];
		return (1);
	}
	return (0);
}

int	helper3(t_exp_helper *expand, int exit_status)
{
	if (expand->original[expand->i] == '?')
	{
		expand->var_value = ft_itoa(exit_status);
		expand->i++;
		return (1);
	}
	return (0);
}
 
// int	expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env) old 0
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

// int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env)  old 1
// {
//     if (expand->original[expand->i] == '$' && expand->quote_state != 1)
//     {
//         expand->i++;
//         if (!helper3(expand, exit_status))
//         {
//             expand->start = expand->i;
//             while (expand->original[expand->i]
//                 && is_valid_var_char(expand->original[expand->i]))
//                 expand->i++;
//             expand->var_name = malloc(expand->i - expand->start + 1);
//             strncpy(expand->var_name, expand->original + expand->start,
//                 expand->i - expand->start);
//             expand->var_name[expand->i - expand->start] = '\0';
//             expand->var_value = lookup_variable(expand->var_name, env);
//             free(expand->var_name);
//         }
        
//         // Check if we should apply word splitting
//         if (expand->quote_state == 0 && expand->var_value) {
//             // Don't split in assignment context
//             if (!is_in_assignment(expand->original, expand->i)) {
//                 // Try to split - will return NULL if splitting not needed
//                 expand->split_words = split_if_needed(expand->var_value);
                
//                 if (expand->split_words) {
//                     // Splitting happened - copy first word
//                     expand->split_occurred = 1;
//                     expand->k = 0;
//                     while (expand->split_words[0][expand->k])
//                         expand->expanded[expand->j++] = expand->split_words[0][expand->k++];
                    
//                     free(expand->var_value);
//                     return (1);
//                 }
//             }
//         }
        
//         // No splitting needed - copy value as-is
//         expand->k = 0;
//         while (expand->var_value && expand->var_value[expand->k])
//             expand->expanded[expand->j++] = expand->var_value[expand->k++];
        
//         free(expand->var_value);
//         return (1);
//     }
//     return (0); 
// }

// int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env)  old 2
// {
//     if (expand->original[expand->i] == '$' && expand->quote_state != 1)
//     {
//         expand->i++;
//         if (!helper3(expand, exit_status))
//         {
//             expand->start = expand->i;
//             while (expand->original[expand->i]
//                 && is_valid_var_char(expand->original[expand->i]))
//                 expand->i++;
//             expand->var_name = malloc(expand->i - expand->start + 1);
//             strncpy(expand->var_name, expand->original + expand->start,
//                 expand->i - expand->start);
//             expand->var_name[expand->i - expand->start] = '\0';
//             expand->var_value = lookup_variable(expand->var_name, env);
//             free(expand->var_name);
//         }
        
//         // Check if we should apply word splitting
//         if (expand->quote_state == 0 && expand->var_value) {
//             // Check the assignment context
//             int context = is_in_assignment_context(expand->original, expand->i);
            
//             // Only skip splitting on right side of assignments
//             if (context != 1) {
//                 // Try to split
//                 expand->split_words = split_if_needed(expand->var_value);
                
//                 if (expand->split_words) {
//                     // Splitting happened - copy first word
//                     expand->split_occurred = 1;
//                     expand->k = 0;
//                     while (expand->split_words[0][expand->k])
//                         expand->expanded[expand->j++] = expand->split_words[0][expand->k++];
                    
//                     free(expand->var_value);
//                     return (1);
//                 }
//             }
//         }
        
//         // No splitting needed - copy value as-is
//         expand->k = 0;
//         while (expand->var_value && expand->var_value[expand->k])
//             expand->expanded[expand->j++] = expand->var_value[expand->k++];
        
//         free(expand->var_value);
//         return (1);
//     }
//     return (0);
// }


int expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env)
{
    if (expand->original[expand->i] == '$' && expand->quote_state != 1)
    {
        expand->i++;
        if (!helper3(expand, exit_status))
        {
            expand->start = expand->i;
            while (expand->original[expand->i]
                && is_valid_var_char(expand->original[expand->i]))
                expand->i++;
            expand->var_name = malloc(expand->i - expand->start + 1);
            strncpy(expand->var_name, expand->original + expand->start,
                expand->i - expand->start);
            expand->var_name[expand->i - expand->start] = '\0';
            expand->var_value = lookup_variable(expand->var_name, env);
            free(expand->var_name);
        }
        
        // Simply copy the value as is
        expand->k = 0;
        while (expand->var_value && expand->var_value[expand->k])
            expand->expanded[expand->j++] = expand->var_value[expand->k++];
        
        free(expand->var_value);
        return (1);
    }
    return (0);
}

// void	process_string(char *str, t_exp_helper *expand,   old 1
// 					t_env *env, int exit_status)
// {
// 	if (!expand_fill_str(expand, str))
// 		return ;
// 	while (expand->original[expand->i]) {
// 		if (!expand_handle_helper0(expand)
// 			&& !expand_handle_helper1(expand, exit_status, env))
// 			expand->expanded[expand->j++] = expand->original[expand->i++];
// 	}
// 	expand->expanded[expand->j] = '\0';
// }

// void process_string(char *str, t_exp_helper *expand, old 1
//                    t_env *env, int exit_status)
// {
//     if (!expand_fill_str(expand, str))
//         return;
    
//     // Initialize word splitting fields
//     expand->split_occurred = 0;
//     expand->split_words = NULL;
    
//     while (expand->original[expand->i]) {
//         if (!expand_handle_helper0(expand)
//             && !expand_handle_helper1(expand, exit_status, env))
//             expand->expanded[expand->j++] = expand->original[expand->i++];
//     }
//     expand->expanded[expand->j] = '\0';
// }


void process_string(char *str, t_exp_helper *expand,
                   t_env *env, int exit_status)
{
    if (!expand_fill_str(expand, str))
        return;
    
    while (expand->original[expand->i]) {
        if (!expand_handle_helper0(expand)
            && !expand_handle_helper1(expand, exit_status, env))
            expand->expanded[expand->j++] = expand->original[expand->i++];
    }
    expand->expanded[expand->j] = '\0';
}
// void	expand_handle(t_cmd *cmd_list, t_env *env, int exit_status) old 0 
// {
// 	t_cmd			*current;
// 	t_exp_helper	*expand;
// 	t_redir			*redir;
// 	int				i;

// 	expand = (t_exp_helper *)malloc(sizeof(t_exp_helper));
// 	if (!expand)
// 		return ;
// 	current = cmd_list;
// 	while (current)
// 	{
// 		if (current->cmd)
// 		{
// 			process_string(current->cmd, expand, env, exit_status);
// 			free(current->cmd);
// 			current->cmd = expand->expanded;
// 		}
// 		i = 0;
// 		while (current->args && current->args[i])
// 		{
// 			process_string(current->args[i], expand, env, exit_status);
// 			free(current->args[i]);
// 			current->args[i] = expand->expanded;
// 			i++;
// 		}
// 		redir = current->redirs;
// 		while (redir)
// 		{
// 			if (redir->file)
// 			{
// 				process_string(redir->file, expand, env, exit_status);
// 				free(redir->file);
// 				redir->file = expand->expanded;
// 			}
// 			redir = redir->next;
// 		}
// 		current = current->next;
// 	}
// 	free(expand);
// }



// void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)  // old 1
// {
//     t_cmd *current;
//     t_exp_helper *expand;
//     t_redir *redir;
//     int i;

//     expand = (t_exp_helper *)malloc(sizeof(t_exp_helper));
//     if (!expand)
//         return;
        
//     current = cmd_list;
//     while (current)
//     {
//         // Handle command
//         if (current->cmd)
//         {
//             process_string(current->cmd, expand, env, exit_status);
//             free(current->cmd);
//             current->cmd = expand->expanded;
            
//             // Handle splitting if it occurred
//             if (expand->split_occurred && expand->split_words) {
//                 free_string_array(expand->split_words);
//                 expand->split_words = NULL;
//             }
//         }
        
//         // Handle args
//         i = 0;
//         while (current->args && current->args[i])
//         {
//             // Check if this arg has an equals sign
//             char *equals_pos = strchr(current->args[i], '=');
            
//             process_string(current->args[i], expand, env, exit_status);
//             free(current->args[i]);
//             current->args[i] = expand->expanded;
            
//             // If there was splitting and there's an equals sign
//             if (expand->split_occurred && expand->split_words) {
//                 // Special handling for args with equals sign
//                 if (equals_pos) {
//                     // Find the equals sign in the expanded string
//                     char *new_equals = strchr(expand->expanded, '=');
                    
//                     if (new_equals) {
//                         // For args with equals, only split after the equals sign
//                         // First, convert everything after equals to individual args
//                         int word_count = 0;
//                         while (expand->split_words[word_count])
//                             word_count++;
                        
//                         if (word_count > 1) {
//                             // Count existing args
//                             int arg_count = 0;
//                             while (current->args[arg_count])
//                                 arg_count++;
                            
//                             // Create new args array with space for additional args
//                             char **new_args = malloc(sizeof(char *) * (arg_count + word_count));
//                             if (new_args) {
//                                 // Keep all args before current
//                                 for (int j = 0; j < i; j++)
//                                     new_args[j] = current->args[j];
                                
//                                 // Create combined arg with the part before equals and first word after
//                                 int prefix_len = new_equals - expand->expanded + 1; // +1 to include equals
//                                 char *first_part = malloc(prefix_len + strlen(expand->split_words[0]) + 1);
//                                 if (first_part) {
//                                     strncpy(first_part, expand->expanded, prefix_len);
//                                     strcpy(first_part + prefix_len, expand->split_words[0]);
//                                     new_args[i] = first_part;
//                                 } else {
//                                     new_args[i] = strdup(expand->expanded);
//                                 }
                                
//                                 // Add remaining split words as separate args
//                                 for (int j = 1; j < word_count; j++)
//                                     new_args[i+j] = strdup(expand->split_words[j]);
                                
//                                 // Add remaining original args
//                                 for (int j = i+1; j < arg_count; j++)
//                                     new_args[j+word_count-1] = current->args[j];
                                
//                                 new_args[arg_count+word_count-1] = NULL;
                                
//                                 // Replace args array
//                                 free(current->args);
//                                 current->args = new_args;
                                
//                                 // Skip the newly inserted args
//                                 i += word_count - 1;
//                             }
//                         }
//                     }
//                 } else {
//                     // Regular args (no equals) - handle normally
//                     int word_count = 0;
//                     while (expand->split_words[word_count])
//                         word_count++;
                    
//                     if (word_count > 1) {
//                         // Count existing args
//                         int arg_count = 0;
//                         while (current->args[arg_count])
//                             arg_count++;
                        
//                         // Create new args array
//                         char **new_args = malloc(sizeof(char *) * (arg_count + word_count));
//                         if (new_args) {
//                             // Copy args before current
//                             for (int j = 0; j < i; j++)
//                                 new_args[j] = current->args[j];
                            
//                             // Set current to first word (already expanded)
//                             new_args[i] = current->args[i];
                            
//                             // Insert additional words as separate args
//                             for (int j = 1; j < word_count; j++)
//                                 new_args[i+j] = strdup(expand->split_words[j]);
                            
//                             // Copy remaining args
//                             for (int j = i+1; j < arg_count; j++)
//                                 new_args[j+word_count-1] = current->args[j];
                            
//                             new_args[arg_count+word_count-1] = NULL;
                            
//                             // Update args array
//                             free(current->args);
//                             current->args = new_args;
                            
//                             // Skip the newly inserted arguments
//                             i += word_count - 1;
//                         }
//                     }
//                 }
                
//                 // Free split words
//                 free_string_array(expand->split_words);
//                 expand->split_words = NULL;
//             }
            
//             i++;
//         }
        
//         // Handle redirections
//         redir = current->redirs;
//         while (redir)
//         {
//             if (redir->file)
//             {
//                 process_string(redir->file, expand, env, exit_status);
//                 free(redir->file);
//                 redir->file = expand->expanded;
                
//                 // For redirections, we don't handle splitting
//                 // Just use the first word and ignore others
//                 if (expand->split_occurred && expand->split_words) {
//                     free_string_array(expand->split_words);
//                     expand->split_words = NULL;
//                 }
//             }
//             redir = redir->next;
//         }
        
//         current = current->next;
//     }
    
//     free(expand);
// }

void expand_handle(t_cmd *cmd_list, t_env *env, int exit_status)
{
    t_cmd *current;
    t_exp_helper *expand;
    t_redir *redir;
    int i;

    expand = (t_exp_helper *)malloc(sizeof(t_exp_helper));
    if (!expand)
        return;
        
    current = cmd_list;
    while (current)
    {
        // Handle command
        if (current->cmd)
        {
            process_string(current->cmd, expand, env, exit_status);
            free(current->cmd);
            current->cmd = expand->expanded;
        }
        
        // Handle args
        i = 0;
        while (current->args && current->args[i])
        {
            process_string(current->args[i], expand, env, exit_status);
            free(current->args[i]);
            current->args[i] = expand->expanded;
            i++;
        }
        
        // Handle redirections
        redir = current->redirs;
        while (redir)
        {
            if (redir->file)
            {
                process_string(redir->file, expand, env, exit_status);
                free(redir->file);
                redir->file = expand->expanded;
            }
            redir = redir->next;
        }
        
        current = current->next;
    }
    
    free(expand);
    
    // Now apply word splitting with special rules
    apply_word_splitting(cmd_list);
}