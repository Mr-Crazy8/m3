#include "minishell.h"

void free_cmd_list(t_cmd *cmd_list)
{
    t_cmd *current;
    t_cmd *next;

    current = cmd_list;
    while (current)
    {
        next = current->next;
        free(current->cmd); // Free the separate command name
        // Free args, redirs, etc.
        free(current);
        current = next;
    }
}