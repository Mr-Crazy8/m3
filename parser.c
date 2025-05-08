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
    tmp->fd = open_file(type, file);
    tmp->Ambiguous = 0;
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