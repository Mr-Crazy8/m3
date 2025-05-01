#include "minishell.h" ///5 func

void process_redir_helper(char str, int *quote_state)
{
    if (str == '\'')
        {
            if ((*quote_state) == 0)
                (*quote_state) = 1;
            else if ((*quote_state) == 1)
                (*quote_state) = 0;
        }
    else if (str == '\"')
        {
            if ((*quote_state) == 0)
                (*quote_state) = 2;
            else if ((*quote_state) == 2)
                (*quote_state) = 0;
        }
}


static int find_redir(char *str, int i)
{
    while (str && str[i])
    {
        if ((str[i] == '\'' && str[ft_strlen(str) - 1] == '\''))
            break;
        else  if ((str[i] == '\"' && str[ft_strlen(str) - 1] == '\"'))
            break;
        else if (str[i] == '<' || str[i] == '>')
            return (i);
        i++;
    }
    return (i);
}

static char *join_redir(char *result, char *temp)
{
    char *redir;
    
    if (result)
    {
        redir = ft_strjoin(result, " ");
        free(result);
        result = ft_strjoin(redir, temp);
        free(redir);
        free(temp);
    }
    else
        result = temp;
    return (result);
}

char *redir_extracter(char *str)
{
    int i = 0;
    char *temp;
    char *result = NULL;

    if (!str)
        return (NULL);
    while (str[i])
    {
        i = find_redir(str, i); /// static done
        if (str[i] == '<' || str[i] == '>')
        {
            temp = process_redir(str, &i); // static 
            result = join_redir(result, temp); // static done
        }
        else
            break;
    }
    if (!result)
        result = ft_substr("", 0, 0);
    return (result);
}

static char *process_redir(char *str, int *pos)
{
    int start;
    int i;
    int quote_state;
    
    start = *pos;
    i = start + 1;
    if (str[start] == '>' && str[i] == '>')
        i++;
    else if (str[start] == '<' && str[i] == '<')
        i++;
    while (str[i] == ' ')
        i++;
    
    quote_state = 0;
    while (str[i])
    {
        process_redir_helper(str[i], &quote_state);
        if (quote_state == 0 && (str[i] == ' ' || str[i] == '<' || str[i] == '>'))
            break;
        i++;
    }
    
    *pos = i;
    return (ft_substr(str, start, i - start));
}



