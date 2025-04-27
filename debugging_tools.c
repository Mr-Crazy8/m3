#include "minishell.h"

void	print_tokens(t_token *token_list)
{
	t_token	*current;
	int		i;

	current = token_list;
	i = 0;
	while (current)
	{
		printf("Token %d: DATA=[%s], TYPE=[%s]\n",
			i++, current->DATA, current->TOKIN);
		current = current->next;
	}
}

void	print_env(t_env *env_struct)
{
	t_env *tmp;

	tmp = env_struct;
	while (tmp)
	{
		if (tmp->key && tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}

}


void print_cmd(t_cmd *cmd_list)    /// this should be deleted in the final work
{
    t_cmd *tmp;
    int i = 0;
    tmp = cmd_list;

    while (tmp)
    {
        i = 0;
        printf("command name : %s\n", tmp->cmd);
        while (tmp->args[i])
        {
            printf("command args : %s\n", tmp->args[i]);
            i++;
        }
        if (tmp->redirs)
        {
            t_redir *tp = tmp->redirs;
            printf("redir list\n");
            printf("0:<, 1:>, 2:>>, 3:<< \n");
            while (tp)
            {
                printf("type of redir : %d\n", tp->type);  // Added newline
                printf("file name : %s\n", tp->file);      // Added newline
                tp = tp->next;
            }
        }
        printf("is there any pipe ? : %d\n", tmp->pipe_out);
        printf("\n");
        tmp = tmp->next;
    }   
}

