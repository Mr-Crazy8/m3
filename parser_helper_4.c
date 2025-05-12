#include "minishell.h" //4 func

int creat_redir_list_helper(char *str)
{
    if (str && strcmp(str, ">") != 0
    && strcmp(str, ">>") != 0
    && strcmp(str, "<") != 0
    && strcmp(str, "<<") != 0)
        return 1;
    return 0;
}


t_redir *creat_redir_list_helper0(char *str1, char *str2)
{
    if (strcmp(str1, ">>") == 0 && str2 != NULL
            && creat_redir_list_helper(str2) == 1)
            return(creat_redir_node(2, ft_strdup(str2)));
    else if (strcmp(str1, "<<") == 0 && str2 != NULL
             && creat_redir_list_helper(str2) == 1)
            return(creat_redir_node(3, ft_strdup(str2)));
    else if (strcmp(str1, ">") == 0 && str2 != NULL
                && creat_redir_list_helper(str2) == 1)
                return(creat_redir_node(1, ft_strdup(str2)));
    else if (strcmp(str1, "<") == 0 && str2 != NULL
                && creat_redir_list_helper(str2) == 1)
               return(creat_redir_node(0, ft_strdup(str2)));
    return (NULL);
}

t_redir *creat_redir_list(char *str) //49
{
    t_redir *tmp_node;
    t_redir *tmp_list;
    char **split_str;
    int i;

    i = 0;
    tmp_list = NULL;
    tmp_node = NULL;
    split_str = ft_split_q(redir_extracter(str), ' ');
    if (!split_str)
        return (NULL);
    
    while(split_str[i])
    {
        // Handle case where redirection has no filename or empty filename
        if ((strcmp(split_str[i], ">") == 0 || 
            strcmp(split_str[i], ">>") == 0 ||
            strcmp(split_str[i], "<") == 0 || 
            strcmp(split_str[i], "<<") == 0) && 
            (!split_str[i+1] || split_str[i+1][0] == '\0'))
        {
            // Create node with empty filename
            if (strcmp(split_str[i], ">") == 0)
                tmp_node = creat_redir_node(1, ft_strdup(""));
            else if (strcmp(split_str[i], ">>") == 0)
                tmp_node = creat_redir_node(2, ft_strdup(""));
            else if (strcmp(split_str[i], "<") == 0)
                tmp_node = creat_redir_node(0, ft_strdup(""));
            else if (strcmp(split_str[i], "<<") == 0)
                tmp_node = creat_redir_node(3, ft_strdup(""));
                
            if (tmp_node)
                add_redir_back(&tmp_list, tmp_node);
            i++;
        }
        else
        {
            tmp_node = NULL;
            tmp_node = creat_redir_list_helper0(split_str[i], split_str[i+1]);
            if (tmp_node)
            {
                add_redir_back(&tmp_list, tmp_node);
                i++;
            }
            i++;
        }
    }
    free_split_str(split_str);
    return (tmp_list);
}
