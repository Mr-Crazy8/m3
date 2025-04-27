#include "minishell.h"


// void ff()
// {
//     system("leaks minishell");
// }


int main(int argc, char *argv[], char *env[])
{

    t_token *token_list;
    t_env *env_struct = NULL;   //// add to the final main
    int exit_status;
    char *input;
    t_cmd *cmd;
    
    // atexit(ff);
    env_maker(env, &env_struct);
    token_list = NULL;
    while (1)
    {
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
            
        token_list = tokin_list_maker(input);
        if (token_list && !error_pipi(token_list)  && !check_syntax_errors(token_list))
        {
            printf("--- TOKENS ---\n");
            expand_handle(token_list, env_struct, exit_status);
            //process_quotes_for_tokens(token_list, 1);
            cmd = parser(token_list);
            //debug_print_cmd(cmd);
            if (cmd == NULL) {
                printf("Warning: Command list is empty after parsing!\n");
            } else {
                process_quotes_for_cmd(cmd, 1);
                print_cmd(cmd);
            }
          // print_tokens(token_list);
           // command_table = parse_tokens(token_list);
           //print_command_table(command_table);
            
            // Free the command table here to avoid memory leaks
            // free_command_table(command_table);
        }   
        free_token_list(token_list);
        
        //free_tokens(token_list);
        free(input);
    }
    return 0;
}
