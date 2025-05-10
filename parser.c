#include "minishell.h" //5 func









void skip_redirections_hp(char str, int *quote_state)
{
    if (str == '\'')
    {
        if (*quote_state == 0)
            *quote_state = 1;
        else if (*quote_state == 1)
            *quote_state = 0;
    }
    else if (str == '\"')
    {
        if (*quote_state == 0)
            *quote_state = 2;
        else if (*quote_state == 2)
            *quote_state = 0;
    }
}

static int skip_redirections(char *str)
{
    int i;
    int quote_state; // 0: no quote, 1: single quote, 2: double quote

    quote_state = 0;
    i = 0;
    if (!str)
        return (0);
    while (str[i])
    {
        skip_redirections_hp(str[i], &quote_state);
        if ((str[i] == '<' || str[i] == '>') && quote_state == 0)
        {
            i++;
            while (str[i] && str[i] == ' ')
                i++;
            while (str[i] && str[i] != ' ' && str[i] != '<' && str[i] != '>')
                i++;
        }
        else if (str[i] == ' ' && quote_state == 0)
            i++;
        else
            break;
    }
    return (i);
}

void  cmd_extracter_hp_0(char str, int *quote_state)
{
      if (str == '\'')
        {
            if (*quote_state == 0)
                *quote_state = 1;
            else if (*quote_state == 1)
                *quote_state = 0;
        }
        else if (str == '\"')
        {
            if (*quote_state == 0)
                *quote_state = 2;
            else if (*quote_state == 2)
                *quote_state = 0;
        }

}

void  cmd_extracter_hp_1(char *str, int *quote_state, int *i, int *result_len, char *result)
{
     char quote;
       if (*quote_state == 0 && (str[(*i)] == '>' || str[(*i)] == '<'))
        {
            if (*result_len > 0 && result[(*result_len)-1] != ' ')
                result[(*result_len)++] = ' ';
            (*i)++;
            if (str[(*i)] == '>' || str[(*i)] == '<')
                (*i)++;
            while (str[(*i)] && str[(*i)] == ' ')
                (*i)++;
            while (str[(*i)] && str[(*i)] != ' ' && str[(*i)] != '>' && str[(*i)] != '<')
            {
                if (str[(*i)] == '\'' || str[(*i)] == '\"')
                {
                    quote = str[(*i)++];
                    while (str[(*i)] && str[(*i)] != quote)
                        (*i)++;
                    if (str[(*i)])
                        (*i)++;
                }
                else
                    (*i)++;
            }
        }
}


void cmd_extracter_hp_2(char *str, int *i, char *result, int *result_len)
{
    if (str[(*i)] == ' ')
        {
            result[(*result_len)++] = ' ';
            (*i)++;
            while (str[(*i)] && str[(*i)] == ' ')
                (*i)++;
        }
    else
            result[(*result_len)++] = str[(*i)++];

}

char *cmd_extracter_hp_3(char *result, int *result_len)
{
    char *trimmed;
    char *final;
    int len;

     result[(*result_len)] = '\0';
    trimmed = result;
    while (*trimmed == ' ')
        trimmed++;
    final = ft_strdup(trimmed);
    len = ft_strlen(final);
    while (len > 0 && final[len-1] == ' ')
        final[--len] = '\0';
    free(result);
    return (final);

}

char *init_cmd_buffer(char *str, int *i, int *result_len, int *quote_state)
{
    char *result;
    
    *i = 0;
    *quote_state = 0;
    *result_len = 0;
    
    if (!str)
        return (NULL);
    
    // Allocate and initialize buffer
    result = (char *)malloc(strlen(str) + 1);
    if (!result)
        return (NULL);
    
    result[0] = '\0';
    *i = skip_redirections(str);
    
    return (result);
}

char *cmd_extracter(char *str)
{
    char *result;
    int i;
    int result_len;
    int quote_state;

    result = init_cmd_buffer(str, &i, &result_len, &quote_state);
    if (!result)
        return (NULL);
    while (str[i])
    {
        cmd_extracter_hp_0(str[i], &quote_state);
        if (quote_state == 0 && (str[i] == '>' || str[i] == '<'))
            cmd_extracter_hp_1(str, &quote_state, &i, &result_len, result);
        else
            cmd_extracter_hp_2(str, &i, result, &result_len);
    }
    
    return (cmd_extracter_hp_3(result, &result_len));
}


t_redir *creat_redir_node(int type, char *file)
{
    t_redir *tmp;

    tmp = malloc(sizeof(t_redir));
    if (!tmp)
        return (NULL);
    tmp->file = file;
    tmp->orig_token = ft_strdup(file);
    tmp->fd = -1;
    tmp->Ambiguous = 0;
    tmp->type = type;
    tmp->next = NULL;

    return (tmp);
}


#include <stdlib.h>
#include <string.h>
#include "minishell.h"

// Helper function to extract key from a key=value pair
char *extract_key(const char *assignment)
{
    int i = 0;
    while (assignment[i] && assignment[i] != '=')
        i++;
    
    char *key = malloc(i + 1);
    if (!key)
        return NULL;
    
    strncpy(key, assignment, i);
    key[i] = '\0';
    return key;
}

// Helper function to check if a string contains invalid characters for a variable name
int is_valid_varname(const char *str)
{
    // Variable names should start with a letter or underscore
    if (!(*str == '_' || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z')))
        return 0;
    
    // Rest of the characters should be alphanumeric or underscore
    while (*str)
    {
        if (!(*str == '_' || (*str >= 'a' && *str <= 'z') || 
              (*str >= 'A' && *str <= 'Z') || (*str >= '0' && *str <= '9')))
            return 0;
        str++;
    }
    return 1;
}

int check_for_export_case(char *str, char *before_DATA_expand)
{
    // First check if the command starts with "export"
    if (strncmp(str, "export", 6) != 0)
        return 0;
    
    // Check if the next character is a space or null terminator
    if (str[6] != ' ' && str[6] != '\0')
        return 0;
    
    // Check if the actual command in before_DATA_expand has quotes or $ around "export"
    if (before_DATA_expand) {
        // Check if "export" is quoted or has $ prefix in the original command
        if ((before_DATA_expand[0] == '"' || before_DATA_expand[0] == '\'' || before_DATA_expand[0] == '$') &&
            strstr(before_DATA_expand, "export"))
            return 0;
    }
    
    // We need to examine the original command (before_DATA_expand) to check for $ prefix
    if (before_DATA_expand) {
        // Skip "export" and spaces in the original command
        char *orig_ptr = before_DATA_expand;
        if (strncmp(orig_ptr, "export", 6) == 0) {
            orig_ptr += 6;  // Skip "export"
            
            // Skip spaces
            while (*orig_ptr && *orig_ptr == ' ')
                orig_ptr++;
            
            // Check if variable name starts with $ in the original command
            if (*orig_ptr == '$') {
                return 0;  // Variable starts with $, return 0
            }
            
            // Look for equals sign
            char *orig_equals = strchr(orig_ptr, '=');
            if (orig_equals) {
                // Check for quotes in the variable part
                for (char *p = orig_ptr; p < orig_equals; p++) {
                    if (*p == '\'' || *p == '"') {
                        return 0;  // Variable name contains quotes in original command
                    }
                }
            }
        }
    }
    
    // Skip "export" and any spaces in the expanded command
    char *ptr = str + 6;
    while (*ptr && *ptr == ' ')
        ptr++;
    
    // Check if we have an assignment operation
    char *equals = strchr(ptr, '=');
    if (!equals)
        return 0;  // No assignment found
    
    // If we got here, it's a valid export case
    return 1;
}

// Function to handle the export case after expansion
char **export_case_handler(char *str)
{
    // Use original tokens from before_DATA_expand to determine token boundaries
    char **args = NULL;
    int arg_count = 1; // Start with 1 for "export"
    
    // Skip "export" and count the number of assignments
    char *ptr = str + 6;
    while (*ptr && *ptr == ' ')
        ptr++;
    
    // Count how many assignments we need to handle
    char *token_start = ptr;
    while (*token_start)
    {
        // Skip to next non-space character
        while (*token_start && *token_start == ' ')
            token_start++;
            
        if (!*token_start)
            break;
            
        // Found start of a token, count it
        arg_count++;
        
        // Find the equals sign
        char *equals = strchr(token_start, '=');
        if (!equals)
        {
            // No equals sign, just skip to next space
            while (*token_start && *token_start != ' ')
                token_start++;
            continue;
        }
        
        // Skip to the next token boundary
        // For simplicity, we'll consider everything up to the next unquoted space as one token
        char *value_ptr = equals + 1;
        int in_quotes = 0;
        char quote_char = 0;
        
        while (*value_ptr)
        {
            if (!in_quotes && *value_ptr == ' ')
                break; // Unquoted space means end of this assignment
                
            if (*value_ptr == '\'' || *value_ptr == '"')
            {
                if (!in_quotes)
                {
                    in_quotes = 1;
                    quote_char = *value_ptr;
                }
                else if (*value_ptr == quote_char)
                {
                    in_quotes = 0;
                }
            }
            
            value_ptr++;
        }
        
        token_start = value_ptr;
    }
    
    // Allocate space for args array
    args = malloc(sizeof(char *) * (arg_count + 1)); // +1 for NULL terminator
    if (!args)
        return NULL;
        
    // First argument is always "export"
    args[0] = ft_strdup("export");
    if (!args[0])
    {
        free(args);
        return NULL;
    }
    
    // Now extract each assignment
    int i = 1;
    token_start = ptr;
    
    while (*token_start && i < arg_count)
    {
        // Skip to next non-space character
        while (*token_start && *token_start == ' ')
            token_start++;
            
        if (!*token_start)
            break;
            
        // Find the equals sign
        char *equals = strchr(token_start, '=');
        if (!equals)
        {
            // No equals sign, just skip to next space
            char *token_end = token_start;
            while (*token_end && *token_end != ' ')
                token_end++;
                
            // Copy this token
            int len = token_end - token_start;
            args[i] = malloc(len + 1);
            if (!args[i])
            {
                // Free everything allocated so far
                while (i >= 0)
                {
                    free(args[i]);
                    i--;
                }
                free(args);
                return NULL;
            }
            
            strncpy(args[i], token_start, len);
            args[i][len] = '\0';
            i++;
            
            token_start = token_end;
            continue;
        }
        
        // Extract and validate the key
        int key_len = equals - token_start;
        char *key = malloc(key_len + 1);
        if (!key)
        {
            // Free everything allocated so far
            while (i > 0)
            {
                free(args[i-1]);
                i--;
            }
            free(args);
            return NULL;
        }
        
        strncpy(key, token_start, key_len);
        key[key_len] = '\0';
        
        if (!is_valid_varname(key))
        {
            free(key);
            // Skip this invalid assignment
            while (*token_start && *token_start != ' ')
                token_start++;
            continue;
        }
        free(key);
        
        // Determine the end of this assignment
        char *value_ptr = equals + 1;
        int in_quotes = 0;
        char quote_char = 0;
        
        while (*value_ptr)
        {
            if (!in_quotes && *value_ptr == ' ')
                break; // Unquoted space means end of this assignment
                
            if (*value_ptr == '\'' || *value_ptr == '"')
            {
                if (!in_quotes)
                {
                    in_quotes = 1;
                    quote_char = *value_ptr;
                }
                else if (*value_ptr == quote_char)
                {
                    in_quotes = 0;
                }
            }
            
            value_ptr++;
        }
        
        // Calculate the total length of this assignment
        int assign_len = value_ptr - token_start;
        
        // For variable expansions with spaces, ensure we get the full value
        if (*equals == '=' && *(equals+1) != '\0' && !strchr(equals+1, ' '))
        {
            // Check if the rest of the string might be part of this value
            char *next_space = strchr(value_ptr, ' ');
            char *next_equals = strchr(value_ptr, '=');
            
            // If there's no next equals sign, or it comes after the next space,
            // then everything up to that space is part of this value
            if (next_equals == NULL || (next_space != NULL && next_space < next_equals))
            {
                while (*value_ptr && *value_ptr != ' ')
                    value_ptr++;
                    
                assign_len = value_ptr - token_start;
            }
        }
        
        // Copy this assignment
        args[i] = malloc(assign_len + 1);
        if (!args[i])
        {
            // Free everything allocated so far
            while (i > 0)
            {
                free(args[i-1]);
                i--;
            }
            free(args);
            return NULL;
        }
        
        strncpy(args[i], token_start, assign_len);
        args[i][assign_len] = '\0';
        i++;
        
        // Move to the next token
        token_start = value_ptr;
    }
    
    // NULL-terminate the array
    args[i] = NULL;
    
    return args;
}




t_cmd *creat_cmd_node(char *str,  t_token *tp,  int pipe_out)
{
    t_cmd *tmp;
    int status = 0;
    tmp = malloc(sizeof(t_cmd));
    if (!tmp)
        return (NULL);
    status = check_for_export_case(str, tp->befor_DATA_expand);
    printf("str =============> %s\n", str);
    printf("status ==========> %d\n", status);
    // if (status == 1)
    // {
    //     printf("str :::::::> %s\n", str);
    //     tmp->args = export_case_handler(str);
    //     printf("1\n");
    // }
    // else
    // {
        tmp->args = ft_split_q(cmd_extracter(str), ' ');
    if (tmp->args && tmp->args[0])
        tmp->cmd = ft_strdup(tmp->args[0]);
    else
        tmp->cmd = NULL;
    tmp->args_befor_quotes_remover = ft_split_q(cmd_extracter(str), ' ');
    tmp->redirs = creat_redir_list(str);
    tmp->pipe_out = pipe_out;
    tmp->next = NULL;
    return (tmp);
}


t_cmd *parser(t_token *token_list)
{
    t_cmd *cmd_node;
    t_cmd *cmd_list = NULL;

    t_token *tmp = token_list;

    while(tmp)
    {
        if (strcmp(tmp->TOKIN, "word_tokin") == 0)
        {
            if (tmp->next && strcmp(tmp->next->TOKIN, "pipe_token") == 0) 
                cmd_node = creat_cmd_node(tmp->DATA,  tmp, 1);
            else 
                cmd_node = creat_cmd_node(tmp->DATA, tmp, 0);
            if (cmd_node)
                add_cmd_back(&cmd_list, cmd_node);
        }
        tmp = tmp->next;
    }
    return (cmd_list);
}