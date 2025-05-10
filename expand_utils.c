#include "minishell.h"

/**
 * Check if a string needs to be split (contains IFS characters)
 * Only returns a split array if actual splitting is needed
 */
char **split_if_needed(char *str)
{
    // Don't split empty strings or strings without IFS characters
    if (!str || !*str || (!strchr(str, ' ') && !strchr(str, '\t') && !strchr(str, '\n')))
        return NULL;
    
    // Count the number of words
    int count = 0;
    int in_word = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
    }
    
    // If only one word, no splitting needed
    if (count <= 1)
        return NULL;
    
    // Allocate array for words
    char **result = malloc(sizeof(char *) * (count + 1));
    if (!result)
        return NULL;
    
    // Split the string
    int j = 0;
    in_word = 0;
    int start = 0;
    
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
            if (in_word) {
                result[j] = strndup(str + start, i - start);
                j++;
                in_word = 0;
            }
        } else if (!in_word) {
            start = i;
            in_word = 1;
        }
    }
    
    // Last word
    if (in_word)
        result[j++] = strdup(str + start);
    
    result[j] = NULL;
    return result;
}

/**
 * Free an array of strings
 */
void free_string_array(char **array)
{
    if (!array)
        return;
    
    for (int i = 0; array[i]; i++)
        free(array[i]);
    
    free(array);
}

/**
 * Check if a character is valid for a variable name
 */
// int is_valid_var_char2(char c)
// {
//     return (isalnum(c) || c == '_');
// }

/**
 * More precise check for assignment context
 * Returns:
 * 0 - Not in an assignment
 * 1 - Right side of an assignment (no splitting)
 * 2 - Left side of an assignment or normal context (do splitting)
 */
/**
 * Check if a position in a string is after an equals sign
 */
int is_in_assignment(char *str, int pos)
{
    int i = 0;
    int in_quotes = 0; // 0 = no quotes, 1 = single quotes, 2 = double quotes
    
    // Find unquoted equals sign before pos
    while (i < pos && str[i]) {
        if (str[i] == '\'' && (in_quotes == 0 || in_quotes == 1))
            in_quotes = (in_quotes == 0) ? 1 : 0;
        else if (str[i] == '"' && (in_quotes == 0 || in_quotes == 2))
            in_quotes = (in_quotes == 0) ? 2 : 0;
        else if (str[i] == '=' && in_quotes == 0) {
            // Found unquoted equals sign before pos
            return 1;
        }
        i++;
    }
    
    return 0;
}



/**
 * Check if this is a special export case where words should be split
 * Returns 1 if splitting should happen, 0 otherwise
 */
/**
 * Check if this is a special export case that requires word splitting
 * Returns: 1 if splitting should happen, 0 otherwise
 */
int is_special_export_case(t_cmd *cmd)
{
    // Not a command with arguments
    if (!cmd || !cmd->cmd || !cmd->args || !cmd->args[0])
        return 0;
        
    // Not an export command
    if (strcmp(cmd->cmd, "export") != 0)
        return 1;  // Not 'export' exactly - split
        
    // Check original command name (before quote removal)
    if (cmd->args_befor_quotes_remover && cmd->args_befor_quotes_remover[0]) {
        char *orig_cmd = cmd->args_befor_quotes_remover[0];
        
        // Check if the original had quotes or variables
        if (strcmp(orig_cmd, "export") != 0) {
            return 1;  // Had quotes or variables - split
        }
    }
    
    // It's a plain export command, now check each argument
    int i = 1;
    while (cmd->args[i]) {
        char *equals = strchr(cmd->args[i], '=');
        if (equals) {
            // It's an assignment, check if variable name had quotes/vars
            if (cmd->args_befor_quotes_remover && cmd->args_befor_quotes_remover[i]) {
                char *orig_arg = cmd->args_befor_quotes_remover[i];
                
                // Find equals in original arg
                char *orig_equals = strchr(orig_arg, '=');
                if (orig_equals) {
                    // Check if part before equals had quotes or $
                    int name_len = orig_equals - orig_arg;
                    for (int j = 0; j < name_len; j++) {
                        if (orig_arg[j] == '\'' || orig_arg[j] == '"' || orig_arg[j] == '$') {
                            return 1;  // Variable name had quotes or $ - split
                        }
                    }
                }
            }
        }
        i++;
    }
    
    return 0;  // Default - don't split for regular export assignments
}







void split_args_for_cmd(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list;
    
    while (current)
    {
        // Check if this is a command that needs splitting
        if (current->args && current->args[0])
        {
            int is_command = (current->cmd != NULL);
            
            // Process each argument
            int i = is_command ? 1 : 0; // Skip the command name if it exists
            while (current->args[i])
            {
                // Check if there's an equals sign and we should split after it
                char *equals = strchr(current->args[i], '=');
                int is_assignment = (equals != NULL);
                
                // Only process certain cases for splitting:
                // 1. Normal commands (not assignments) - split all args
                // 2. For exports with assignments - don't split
                
                if (!is_assignment)
                {
                    // Not an assignment, always split on spaces
                    char **split = split_if_needed(current->args[i]);
                    if (split && split[1]) // If there's more than one word
                    {
                        // Count total args
                        int arg_count = 0;
                        while (current->args[arg_count])
                            arg_count++;
                            
                        // Count split words
                        int word_count = 0;
                        while (split[word_count])
                            word_count++;
                            
                        // Create new args array
                        char **new_args = malloc(sizeof(char *) * (arg_count + word_count));
                        if (new_args)
                        {
                            // Copy args before current
                            for (int j = 0; j < i; j++)
                                new_args[j] = current->args[j];
                                
                            // Replace current with first split word
                            free(current->args[i]);
                            new_args[i] = strdup(split[0]);
                            
                            // Add remaining split words
                            for (int j = 1; j < word_count; j++)
                                new_args[i+j] = strdup(split[j]);
                                
                            // Copy remaining args
                            for (int j = i+1; j < arg_count; j++)
                                new_args[j+word_count-1] = current->args[j];
                                
                            new_args[arg_count+word_count-1] = NULL;
                            
                            // Replace args array
                            free(current->args);
                            current->args = new_args;
                            
                            // Skip the newly added words
                            i += word_count - 1;
                        }
                        
                        free_string_array(split);
                    }
                    else if (split)
                    {
                        free_string_array(split);
                    }
                }
                
                i++;
            }
        }
        
        current = current->next;
    }
}



/**
 * Apply word splitting to command arguments based on specific rules
 */
/**
 * Apply word splitting to command arguments based on specific rules
 */
void apply_word_splitting(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list;
    
    while (current)
    {
        // First, handle the case where the command itself needs splitting
        // This is when args[0] equals cmd, meaning it's the primary command
        if (current->args && current->args[0] && current->cmd && 
            strcmp(current->args[0], current->cmd) == 0) {
            
            // Check if the command contains spaces
            char **split = split_if_needed(current->cmd);
            if (split && split[1]) {  // Only process if it actually splits
                // Free the old command string
                free(current->cmd);
                
                // Set command to the first word
                current->cmd = strdup(split[0]);
                
                // Count words
                int word_count = 0;
                while (split[word_count])
                    word_count++;
                    
                // Count args
                int arg_count = 0;
                while (current->args[arg_count])
                    arg_count++;
                    
                // Create new args array
                char **new_args = malloc(sizeof(char *) * (arg_count + word_count));
                if (new_args) {
                    // First arg is the command
                    new_args[0] = strdup(split[0]);
                    
                    // Add remaining words from the split
                    for (int j = 1; j < word_count; j++)
                        new_args[j] = strdup(split[j]);
                        
                    // Copy remaining original args
                    for (int j = 1; j < arg_count; j++)
                        new_args[j+word_count-1] = current->args[j];
                        
                    new_args[arg_count+word_count-1] = NULL;
                    
                    // Free old args including strings
                    for (int j = 0; j < arg_count; j++)
                        free(current->args[j]);
                    free(current->args);
                    
                    // Set new args
                    current->args = new_args;
                }
            }
            
            if (split)
                free_string_array(split);
        }
        
        // Detect if this is a special case that requires splitting for args
        int should_split = 1;  // Default - do split
        
        // Check if this is an export command with variable assignments
        if (current->cmd && strcmp(current->cmd, "export") == 0) {
            should_split = is_special_export_case(current);
        }
        
        // Only proceed with splitting arguments if needed
        if (should_split && current->args) {
            // For commands, split all args except the first one (which is the command)
            int i = 1;  // Start with args[1]
            while (current->args[i]) {
                // Don't split assignments unless needed
                char *equals = strchr(current->args[i], '=');
                if (!equals || should_split) {
                    // Split this argument
                    char **split = split_if_needed(current->args[i]);
                    if (split && split[1]) {  // Only process if it actually splits
                        // Count words
                        int word_count = 0;
                        while (split[word_count])
                            word_count++;
                            
                        // Count args
                        int arg_count = 0;
                        while (current->args[arg_count])
                            arg_count++;
                            
                        // Create new args array
                        char **new_args = malloc(sizeof(char *) * (arg_count + word_count));
                        if (new_args) {
                            // Copy args before current
                            for (int j = 0; j < i; j++)
                                new_args[j] = current->args[j];
                                
                            // Set current to first word
                            free(current->args[i]);
                            new_args[i] = strdup(split[0]);
                            
                            // Add remaining words
                            for (int j = 1; j < word_count; j++)
                                new_args[i+j] = strdup(split[j]);
                                
                            // Copy remaining args
                            for (int j = i+1; j < arg_count; j++)
                                new_args[j+word_count-1] = current->args[j];
                                
                            new_args[arg_count+word_count-1] = NULL;
                            
                            // Replace args
                            free(current->args);
                            current->args = new_args;
                            
                            // Skip the newly added words
                            i += word_count - 1;
                        }
                    }
                    
                    if (split)
                        free_string_array(split);
                }
                
                i++;
            }
        }
        
        current = current->next;
    }
}