#include "minishell.h" //5 func


static int skip_redirections(char *str)//37
{
    int i;
    int quote_state = 0; // 0: no quote, 1: single quote, 2: double quote

    i = 0;
    if (!str)
        return (0);
    while (str[i])
    {
        // Handle quotes
        if (str[i] == '\'')
        {
            if (quote_state == 0)
                quote_state = 1;
            else if (quote_state == 1)
                quote_state = 0;
        }
        else if (str[i] == '\"')
        {
            if (quote_state == 0)
                quote_state = 2;
            else if (quote_state == 2)
                quote_state = 0;
        }
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

char *cmd_extracter(char *str)///106
{
    char *result = NULL;
    int i = 0;
    int start = 0;
    int quote_state = 0; // 0: no quote, 1: single quote, 2: double quote
    
    if (!str)
        return (NULL);
    
    // Allocate buffer for result (potentially the same size as input)
    result = (char *)malloc(strlen(str) + 1);
    if (!result)
        return (NULL);
    
    // Initialize with empty string
    result[0] = '\0';
    int result_len = 0;
    
    // Skip initial redirections
    i = skip_redirections(str);
    
    while (str[i])
    {
        // Handle quotes
        if (str[i] == '\'')
        {
            if (quote_state == 0)
                quote_state = 1;
            else if (quote_state == 1)
                quote_state = 0;
        }
        else if (str[i] == '\"')
        {
            if (quote_state == 0)
                quote_state = 2;
            else if (quote_state == 2)
                quote_state = 0;
        }
        
        // If we encounter a redirection outside quotes
        if (quote_state == 0 && (str[i] == '>' || str[i] == '<'))
        {
            // Mark end of current argument
            if (result_len > 0 && result[result_len-1] != ' ')
                result[result_len++] = ' ';
                
            // Skip the redirection character
            i++;
            
            // Skip second redirection character if >> or <<
            if (str[i] == '>' || str[i] == '<')
                i++;
                
            // Skip spaces
            while (str[i] && str[i] == ' ')
                i++;
                
            // Skip the filename
            while (str[i] && str[i] != ' ' && str[i] != '>' && str[i] != '<')
            {
                // Handle quoted filenames
                if (str[i] == '\'' || str[i] == '\"')
                {
                    char quote = str[i++];
                    while (str[i] && str[i] != quote)
                        i++;
                    if (str[i])
                        i++;
                }
                else
                    i++;
            }
        }
        else if (str[i] == ' ')
        {
            // Add a single space
            result[result_len++] = ' ';
            i++;
            
            // Skip multiple spaces
            while (str[i] && str[i] == ' ')
                i++;
        }
        else
        {
            // Copy regular characters
            result[result_len++] = str[i++];
        }
    }
    
    // Null-terminate the result
    result[result_len] = '\0';
    
    // Trim leading/trailing spaces
    char *trimmed = result;
    while (*trimmed == ' ')
        trimmed++;
    
    char *final = ft_strdup(trimmed);
    
    // Remove trailing spaces
    int len = strlen(final);
    while (len > 0 && final[len-1] == ' ')
        final[--len] = '\0';
    
    free(result);
    return final;
}


t_redir *creat_redir_node(int type, char *file)
{
    t_redir *tmp;

    tmp = malloc(sizeof(t_redir));
    if (!tmp)
        return (NULL);
    tmp->file = file;
    tmp->type = type;
    tmp->next = NULL;

    return (tmp);
}


t_cmd *creat_cmd_node(char *str, int pipe_out)
{
    t_cmd *tmp;

    tmp = malloc(sizeof(t_cmd));
    if (!tmp)
        return (NULL);
    tmp->args = ft_split_q(cmd_extracter(str), ' ');
    if (tmp->args && tmp->args[0])
        tmp->cmd = ft_strdup(tmp->args[0]);
    else
        tmp->cmd = NULL;
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
                cmd_node = creat_cmd_node(tmp->DATA, 1);
            else 
                cmd_node = creat_cmd_node(tmp->DATA, 0);
            if (cmd_node)
                add_cmd_back(&cmd_list, cmd_node);
        }
        tmp = tmp->next;
    }
    return (cmd_list);
}