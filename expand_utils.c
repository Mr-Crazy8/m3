#include "minishell.h"

/**
 * Check if a string needs to be split (contains IFS characters)
 * Only returns a split array if actual splitting is needed
 */
char **split_if_needed(char *str)
{
    // Don't split empty strings or strings without IFS characters
    if (!str || !*str || (!strchr(str, ' ') && !strchr(str, '\t') && !strchr(str, '\n')))
        return NULL;
    
    // Count the number of words
    int count = 0;
    int in_word = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
    }
    
    // If only one word, no splitting needed
    if (count <= 1)
        return NULL;
    
    // Allocate array for words
    char **result = malloc(sizeof(char *) * (count + 1));
    if (!result)
        return NULL;
    
    // Split the string
    int j = 0;
    in_word = 0;
    int start = 0;
    
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
            if (in_word) {
                result[j] = strndup(str + start, i - start);
                j++;
                in_word = 0;
            }
        } else if (!in_word) {
            start = i;
            in_word = 1;
        }
    }
    
    // Last word
    if (in_word)
        result[j++] = strdup(str + start);
    
    result[j] = NULL;
    return result;
}

/**
 * Free an array of strings
 */
void free_string_array(char **array)
{
    if (!array)
        return;
    
    for (int i = 0; array[i]; i++)
        free(array[i]);
    
    free(array);
}

/**
 * Check if a character is valid for a variable name
 */
int is_valid_var_char(char c)
{
    return (isalnum(c) || c == '_');
}

/**
 * Check if a position in a string is part of an assignment
 * Returns 1 if it is (after the = in var=value), 0 otherwise
 */
int is_in_assignment(char *str, int pos)
{
    int i = 0;
    int in_quotes = 0; // 0 = no quotes, 1 = single quotes, 2 = double quotes
    
    // Find unquoted equals sign before pos
    while (i < pos && str[i]) {
        if (str[i] == '\'' && (in_quotes == 0 || in_quotes == 1))
            in_quotes = (in_quotes == 0) ? 1 : 0;
        else if (str[i] == '"' && (in_quotes == 0 || in_quotes == 2))
            in_quotes = (in_quotes == 0) ? 2 : 0;
        else if (str[i] == '=' && in_quotes == 0) {
            // Found unquoted equals - check if pos is after it
            return (i < pos);
        }
        i++;
    }
    
    return 0;
}