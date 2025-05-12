#include "minishell.h"

char **split_if_needed(char *str)
{
    int i = 0;
    int count;
    char **result;

    i = 0;
    if (!str || !*str || (!strchr(str, ' ') && !strchr(str, '\t') && !strchr(str, '\n')))
        return NULL;
    result = ft_split(str, ' ');
    count = 0;
    if (result) 
    {
        while (result[count])
            count++;
    }
    
    if (count <= 1) 
    {
        free_string_array(result);
        return NULL;
    }
    
    return result;
}

void free_string_array(char **array)
{
    if (!array)
        return;

    int i = 0;

    while (array[i])
        {
            free(array[i]);
            i++;
        }
    
    free(array);
}





// int is_special_export_case(t_cmd *cmd)
// {
//     int i;
//     char *equals;
//     char *orig_arg;
//     char *orig_equals;
//     // int name_len;
//     int j;
//     if (!cmd || !cmd->cmd || !cmd->args || !cmd->args[0])
//         return 0;
//     if (strcmp(cmd->cmd, "export") != 0)
//         return 1;  
//     if (cmd->args_befor_quotes_remover && cmd->args_befor_quotes_remover[0]) 
//     {
//         char *orig_cmd = cmd->args_befor_quotes_remover[0];
//         if (strcmp(orig_cmd, "export") != 0) 
//             return 1;  // Had quotes or variables - split
//     }
//     i = 1;
//     while (cmd->args[i]) 
//     {
//         equals = strchr(cmd->args[i], '=');
//         if (equals) 
//         {
//             if (cmd->args_befor_quotes_remover && cmd->args_befor_quotes_remover[i]) 
//             {
//                 orig_arg = cmd->args_befor_quotes_remover[i];
//                 orig_equals = strchr(orig_arg, '=');
//                 if (orig_equals) 
//                 {
//                     // name_len = orig_equals - orig_arg;
//                     j = 0;
//                     while (j < (orig_equals - orig_arg))
//                     {
//                         if (orig_arg[j] == '\'' || orig_arg[j] == '"' || orig_arg[j] == '$') 
//                             return 1;  // Variable name had quotes or $ - split
//                         j++;
//                     }
//                 }
//             }
//         }
//         i++;
//     }
//     return 0;  // Default - don't split for regular export assignments
// }


int check_var_quotes(char *orig_arg, char *orig_equals)
{
    int j;
    
    j = 0;
    while (j < (orig_equals - orig_arg))
    {
        if (orig_arg[j] == '\'' || orig_arg[j] == '"' || orig_arg[j] == '$') 
            return 1;  // Variable name had quotes or $ - split
        j++;
    }
    return 0;
}

int check_export_cmd(t_cmd *cmd)
{
    if (!cmd || !cmd->cmd || !cmd->args || !cmd->args[0])
        return 0;
    if (strcmp(cmd->cmd, "export") != 0)
        return 1;  
    if (cmd->args_befor_quotes_remover && cmd->args_befor_quotes_remover[0]) 
    {
        char *orig_cmd = cmd->args_befor_quotes_remover[0];
        if (strcmp(orig_cmd, "export") != 0) 
            return 1;  // Had quotes or variables - split
    }
    return 2;  // Continue checking
}

int is_special_export_case(t_cmd *cmd)
{
    int i;
    char *equals;
    char *orig_arg;
    char *orig_equals;
    int cmd_check;
    
    cmd_check = check_export_cmd(cmd);
    if (cmd_check == 0 || cmd_check == 1)
        return cmd_check;
    i = 1;
    while (cmd->args[i]) 
    {
        equals = strchr(cmd->args[i], '=');
        if (equals && cmd->args_befor_quotes_remover && cmd->args_befor_quotes_remover[i]) 
        {
            orig_arg = cmd->args_befor_quotes_remover[i];
            orig_equals = strchr(orig_arg, '=');
            if (orig_equals && check_var_quotes(orig_arg, orig_equals))
                return 1;
        }
        i++;
    }
    return 0;
}


int ft_lint(char **str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}




void cmd_splitting_helper(t_cmd *current, char **new_args, char **split, int word_count, int arg_count)
{
    int j;

    new_args[0] = strdup(split[0]);
    j = 1;
    while (j < word_count) 
        {
            new_args[j] = strdup(split[j]);
                    j++;
        }
        j = 1;
        while (j < arg_count) 
            {
                new_args[j+word_count-1] = current->args[j];
                    j++;
            }
            new_args[arg_count+word_count-1] = NULL;
            j = 0;
            while (j < arg_count) 
                {
                    free(current->args[j]);
                    j++;
                }
                free(current->args);
                current->args = new_args;
}

void prepare_new_args(char **new_args, t_cmd *current, int i)
{
    int j = 0;
    
    while (j < i)
    {
        new_args[j] = current->args[j];
        j++;
    }
    free(current->args[i]);
}

int rebuild_cmd_args(char **new_args, t_cmd *current, char **split, 
                    int i, int word_count)
{
    int j;
    int arg_count;
    
    j = 1;
    arg_count = ft_lint(current->args);
    new_args[i] = strdup(split[0]);
    while (j < word_count)
    {
        new_args[i+j] = strdup(split[j]);
        j++;
    }
    j = i+1;
    while (j < arg_count)
    {
        new_args[j+word_count-1] = current->args[j];
        j++;
    }
    new_args[arg_count+word_count-1] = NULL;
    free(current->args);
    current->args = new_args;
    
    return i + word_count - 1;
}

void split_the_rest_helper(char *equals, int should_split, t_cmd *current, int *i)
{
    char **split;
    int word_count;
    char **new_args;

    if (!equals || should_split)
    {
            split = split_if_needed(current->args[(*i)]);
            if (split && split[1])
            {
                word_count = ft_lint(split);
                new_args = malloc(sizeof(char *) * 
                           (ft_lint(current->args) + word_count));
                
                if (new_args)
                {
                    prepare_new_args(new_args, current, (*i));
                    (*i) = rebuild_cmd_args(new_args, current, split, 
                                        (*i), word_count);
                }
            }
            if (split)
                free_string_array(split);
    }
}

void split_the_rest(t_cmd *current, int should_split)
{
    int i;
    char *equals;

    if (!should_split || !current->args)
        return;
    i = 1;
    while (current->args[i])
    {
        equals = strchr(current->args[i], '=');
        split_the_rest_helper(equals, should_split, current, &i);
        i++;
    }
}

void cmd_splitting(t_cmd *cmd_list )
{
     t_cmd *current = cmd_list;
     int word_count;
     int arg_count;
     char **new_args;
     char **split;

     if (current->args && current->args[0] && current->cmd &&
            strcmp(current->args[0], current->cmd) == 0) 
        {
            split = split_if_needed(current->cmd);
            if (split && split[1]) 
            { 
                free(current->cmd);
                current->cmd = strdup(split[0]);
                word_count = ft_lint(split);
                arg_count = ft_lint(current->args);
                new_args = malloc(sizeof(char *) * (arg_count + word_count));
                if (new_args)
                    cmd_splitting_helper(current, new_args, split, word_count, arg_count);
            }
            if (split)
                free_string_array(split);
        } 
}


void apply_word_splitting(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list;
    int should_split;
    while (current)
    {
        cmd_splitting(current);
        should_split = 1; 
        if (current->cmd && strcmp(current->cmd, "export") == 0) 
        {
            should_split = is_special_export_case(current);
        }
        split_the_rest(current, should_split);
        current = current->next;
    }
}
