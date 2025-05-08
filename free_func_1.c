#include "minishell.h"

void free_redirs(t_redir *redir_list)
{
    t_redir *current;
    t_redir *next;

    current = redir_list;

    while(current)
    {
        next = current->next;
        if (current->file)
            free(current->file);
        if (current->orig_token)
            free(current->orig_token);
        free(current);
        current = next;
    }
}


void free_cmd_list(t_cmd *cmd_list)
{
    t_cmd *current;
    t_cmd *next;

    current = cmd_list;
    while (current)
    {
        next = current->next;
        if (current->cmd)
            free(current->cmd);
        int i = 0;
        while(current->args[i])
            {
                free(current->args[i]);
                i++;
            }
        free(current->args);
        if (current->redirs)
            free_redirs(current->redirs);
        free(current);
        current = next;
    }
}