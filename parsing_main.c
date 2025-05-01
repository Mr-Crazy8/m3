#include "minishell.h"


char *preprocess_command(char *input) ///73 line 
{
    if (!input)
        return NULL;
        
    // Allocate a buffer - potentially need to add many spaces
    // Worst case: every character could need a space before and after
    char *result = malloc(strlen(input) * 3 + 1);
    if (!result)
        return NULL;
        
    int i = 0;              // input index
    int j = 0;              // result index
    int in_quotes = 0;      // 0: no quotes, 1: single quotes, 2: double quotes
    
    while (input[i])
    {
        // Handle quote state
        if (input[i] == '\'' && in_quotes != 2)  // Not inside double quotes
        {
            if (in_quotes == 1)
                in_quotes = 0;  // Exit single quote mode
            else
                in_quotes = 1;  // Enter single quote mode
                
            result[j++] = input[i++];
            continue;
        }
        else if (input[i] == '\"' && in_quotes != 1)  // Not inside single quotes
        {
            if (in_quotes == 2)
                in_quotes = 0;  // Exit double quote mode
            else
                in_quotes = 2;  // Enter double quote mode
                
            result[j++] = input[i++];
            continue;
        }
        
        // If we're in quotes, copy character as-is
        if (in_quotes)
        {
            result[j++] = input[i++];
            continue;
        }
        
        // Handle redirection operators outside quotes
        if (input[i] == '>' || input[i] == '<')
        {
            // Add space before if the previous char isn't a space or another redirection operator
            if (j > 0 && result[j-1] != ' ' && result[j-1] != '>' && result[j-1] != '<')
                result[j++] = ' ';
                
            // Copy the redirection character
            result[j++] = input[i++];
            
            // Handle >> or << case
            if (input[i] == '>' || input[i] == '<')
                result[j++] = input[i++];
                
            // Add space after
            if (input[i] && input[i] != ' ')
                result[j++] = ' ';
        }
        else
        {
            // Copy regular character
            result[j++] = input[i++];
        }
    }
    
    // Null-terminate the result
    result[j] = '\0';
    
    return result;
}

int main(int argc, char *argv[], char *env[])
{

    t_token *token_list;
    t_env *env_struct = NULL;   //// add to the final main
    int exit_status;
    char *input;
    t_cmd *cmd;
    char *preprocessed_input;
    
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

        // Preprocess input to add spaces around redirection operators
        preprocessed_input = preprocess_command(input);
        free(input);  // Free original input
            
         if (!preprocessed_input)
            continue;


        token_list = tokin_list_maker(preprocessed_input);
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
        free(preprocessed_input);

        //free_tokens(token_list);
        free(input);
    }
    return 0;
}
