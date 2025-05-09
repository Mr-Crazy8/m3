#include "minishell.h"


// Add this function to handle empty redirection targets safely

int preprocess_cmd_hp_0(char *input, int *i, int *in_quotes, char *result, int *j)
{
        if (input[(*i)] == '\'' && *in_quotes != 2)
        {
            if (*in_quotes == 1)
                *in_quotes = 0;
            else
                *in_quotes = 1;
            result[(*j)++] = input[(*i)++];
            return (1);
        }
        else if (input[(*i)] == '\"' && *in_quotes != 1)
        {
            if (*in_quotes == 2)
                *in_quotes = 0; 
            else
                *in_quotes = 2;
            result[(*j)++] = input[(*i)++];
            return (1);
        }
        return(0);

}


void preprocess_cmd_hp_1(char *input, int *i, int *in_quotes, char *result, int *j)
{
    if (input[(*i)] == '>' || input[(*i)] == '<')
    {
        if ((*j) > 0 && result[(*j)-1] != ' ' && result[(*j)-1] != '>' && result[(*j)-1] != '<')
                result[(*j)++] = ' ';
            result[(*j)++] = input[(*i)++];
            if (input[(*i)] == '>' || input[(*i)] == '<')
                result[(*j)++] = input[(*i)++];
            if (input[(*i)] && input[(*i)] != ' ')
                result[(*j)++] = ' ';
    }
    else
        result[(*j)++] = input[(*i)++];

}

int preprocess_cmd_hp_2(char *input, int *i, int *in_quotes, char *result, int *j)
{
    if (*in_quotes)
    {
        result[(*j)++] = input[(*i)++];
        return (1);
    }
    return (0);
}

char *preprocess_command(char *input)
{
    int i = 0;             
    int j = 0;             
    int in_quotes = 0;   
    char *result;
    
    if (!input)
        return NULL;
        
    result = malloc(strlen(input) * 3 + 1);
    if (!result)
        return NULL;
        
    i = 0;             
    j = 0;             
    in_quotes = 0;    
    while (input[i])
    {
        if (preprocess_cmd_hp_0(input, &i, &in_quotes, result, &j))
            continue;
        if (preprocess_cmd_hp_2(input, &i, &in_quotes, result, &j))
            continue;
        preprocess_cmd_hp_1(input, &i, &in_quotes, result, &j);
    }
    return (result[j] = '\0', result);
}

int main(int argc, char *argv[], char *env[])
{
    t_token *token_list;
    t_env *env_struct = NULL;   //// add to the final main
    int exit_status;
    char *input;
    t_cmd *cmd;
    char *preprocessed_input;

    env_maker(env, &env_struct);
    token_list = NULL;
    while (1)
    {
        cmd = NULL;
        input = readline("minishell $> ");
        if (!input)
            break;
        add_history(input);
        if (check_quotes(input))
        {
            // Error message already printed by check_quotes
            free(input);
            continue;
        }
        // Preprocess input to add spaces around redirection operators
        preprocessed_input = preprocess_command(input);
        free(input);  // Free original input
         if (!preprocessed_input)
            continue;
        token_list = tokin_list_maker(preprocessed_input);

        if (token_list && !error_pipi(token_list)  && !check_syntax_errors(token_list))
        {
            expand_handle(token_list, env_struct, exit_status);
            printf("--- TOKENS ---\n");
            //process_quotes_for_tokens(token_list, 1);
            cmd = parser(token_list);
            // expand_handle(cmd, env_struct, exit_status);
            //debug_print_cmd(cmd);

            ambiguous_finder(cmd);
            //print_ambiguous_redir_errors(cmd);
            if (cmd == NULL) {
                printf("Warning: Command list is empty after parsing!\n");
            } else {
                process_quotes_for_cmd(cmd, 1);
                file_opener(cmd);
                print_cmd(cmd);
            }

        }   
        free_token_list(token_list);
        free(preprocessed_input);
    if (cmd)
        free_cmd_list(cmd);
    }
    return 0;
}
