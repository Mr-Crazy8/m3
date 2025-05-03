#include "minishell.h"

//A variable used as the redirection target is empty or unset
//A variable expands to multiple words (creating confusion about which one is the target)


void ambiguous_checker(t_redir *redir)
{
    t_redir *tmp;

    tmp = redir;
    while (tmp)
    {
        if (!tmp->file || tmp->file[0] == '\0')
            tmp->Ambiguous = 1;
        else if (tmp->file && strchr(tmp->file, ' '))
            tmp->Ambiguous = 1;
        tmp = tmp->next;
    }
}


void ambiguous_finder(t_cmd *cmd)
{
    t_cmd *tmp;

    tmp = cmd;
    while(tmp)
    {
        ambiguous_checker(tmp->redirs);
        tmp = tmp->next;
    }
}