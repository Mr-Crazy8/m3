#include "minishell.h" //5 func


// static int skip_redirections(char *str)
// {
//     int i;

//     i = 0;
//     if (!str)
//         return (0);
//     while (str[i])
//     {
//         if (str[i] == '<' || str[i] == '>')
//         {
//             i++;
//             while (str[i] && str[i] == ' ')
//                 i++;
//             while (str[i] && str[i] != ' ' && str[i] != '<' && str[i] != '>')
//                 i++;
//         }
//         else if (str[i] == ' ')
//             i++;
//         else
//             break;
//     }
//     return (i);
// }

// char *cmd_extracter(char *str)
// {
//     int i;
//     int start_cmd;
//     int end_cmd;
//     char *cmd;

//     if (!str)
//         return (NULL);
//     i = skip_redirections(str);
//     start_cmd = i;
//     while (str[i] && str[i] != '>' && str[i] != '<')
//         i++;
//     end_cmd = i;
//     if (end_cmd > start_cmd)
//         cmd = ft_substr(str, start_cmd, end_cmd - start_cmd);
//     else
//         cmd = ft_substr("", 0, 0);
//     return (cmd);
// }


static int skip_redirections(char *str)
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

char *cmd_extracter(char *str)
{
    int i;
    int start_cmd;
    int end_cmd;
    char *cmd;
    int quote_state = 0; // 0: no quote, 1: single quote, 2: double quote

    if (!str)
        return (NULL);
    i = skip_redirections(str);
    start_cmd = i;
    
    while (str[i])
    {
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
        if ((str[i] == '>' || str[i] == '<') && quote_state == 0)
            break;
            
        i++;
    }
    end_cmd = i;
    if (end_cmd > start_cmd)
        cmd = ft_substr(str, start_cmd, end_cmd - start_cmd);
    else
        cmd = ft_substr("", 0, 0);
    return (cmd);
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