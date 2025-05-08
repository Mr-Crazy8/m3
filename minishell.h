#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>  // For printf
#include <stdlib.h> // For malloc
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_token
{
   char *DATA;
   char *TOKIN;
   struct s_token *next;
} t_token;

typedef struct s_env
{
    char    *key;
    char    *value;
    int     defined;
    struct s_env *next;
} t_env;

typedef struct s_var {
    char *name;         // Variable name
    int is_valid;       // Is this a valid variable ref
    int length;         // Length of entire reference
    int name_allocated; // Did we allocate memory for name
} t_var;

typedef struct s_redir
{
    int type;             /* 0:<, 1:>, 2:>>, 3:<< */
    char *file;           /* Filename or heredoc delimiter */
    int fd;
    int Ambiguous;
    char    *orig_token;  /* Original token with quotes */
    char *heredoc_delemter; /* Fully processed delimiter for matching */
    struct s_redir *next; /* Next redirection */
} t_redir;

typedef struct s_cmd
{
    char *cmd;            /* Command name */
    char **args;          /* Command args (including cmd as args[0]) */
    t_redir *redirs;      /* Redirections list */
    int pipe_out;         /* 1 if command pipes to next */
    struct s_cmd *next;   /* Next command in pipeline */
} t_cmd;


typedef struct s_exp_helper {
    
    char *original;
    char *expanded;
    int i;
    int j;
    int quote_state;
    char *var_name;
    char *var_value;
    int k;
    int start;
    
} t_exp_helper;


t_cmd *parse_tokens(t_token *token_list);

//env_helper.c
void	*ft_memcpy(void *dest, const void *src, size_t n);
static size_t	count_words(char const *s, char c);
static char	**split_string(char const *s, char c, char **split, size_t n);
char	**ft_split(char const *s, char c);

//env.c
t_env *env_node_maker(char *env[]);
void env_maker(char *env[], t_env **env_struct);


//lexer_heplper_1.c
int is_whitespace(char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int special_character_cheker(char character);
t_token *special_character_handle(char *input, int *i);
void handle_block_helper(char *input, int *i, t_token **token_list);


//lexer_heplper_0.c
int ft_strlen(const char *str);
char	*ft_strdup(const char *s);
t_token	*creat_token(char *data, char *token_type);
char	*ft_strncpy(char *dest, const char *src, unsigned int n);

//lexer.c
void handle_block(char *input, int *i, int input_length, t_token **token_list);
t_token *tokin_list_maker(char *input);


//syntx_check.c
int error_pipi(t_token *token_list);
int check_syntax_errors(t_token *token_list);
int check_quotes(char *input);
int check_quotes_helper(int in_quote);
int	print_error_pipe(void);

//syntx_check_helper_0.c
int check_for_redirection(char *str);
void  check_missing_filename(char *str);
void print_error(char *str, int *i);
int invalid_filename_checker(char *str, int *i);
int check_invalid_filename(char *str);

//syntx_check_helper_1.c
void	redirection_helper(char *str, int *i, int *count);






int scan_for_expand(char *str);
char *extranct_valu(char *str);
int	ft_isalnum(int c);
int is_valid_var_char(char c);
void	expand_handle(t_cmd *cmd_list, t_env *env, int exit_status);
char *lookup_variable(char *var_name, t_env *env_struct);
int has_unquoted_spaces(char *str);
void split_token(t_token *token);
int get_num_length(long num);
void fill_num_string(char *str, long num, int len);
char *ft_itoa(int n);


void	remove_quotes_from_tokens(t_token *token_list);
char	*remove_quotes(char *str);


char *expand_token_data(char *original, t_env *env, int exit_status);



int get_num_length(long num);
void fill_num_string(char *str, long num, int len);
char *ft_itoa(int n);
int	ft_isalnum(int c);
int is_valid_var_char(char c);
int expand_handle_helper0(t_exp_helper *expand);
int  expand_handle_helper1(t_exp_helper *expand, int exit_status , t_env *env);
int expand_fill(t_exp_helper *expand, t_token *tmp);
void process_token(t_token *tmp, t_exp_helper *expand, t_env *env, int exit_status);
// void expand_handle(t_token *token_list, t_env *env, int exit_status);
char *lookup_variable(char *var_name, t_env *env_struct);



int handle_quotes(char c, int *quote_state);
int special_character_cheker_with_quotes(char character, int quote_state);
char *selective_remove_quotes(char *str, int remove_mode);
//void process_quotes_for_tokens(t_token *token_list, int remove_mode);
int	helper3(t_exp_helper *expand, int exit_status);
int	expand_handle_helper1(t_exp_helper *expand, int exit_status, t_env *env);
void	process_token(t_token *tmp, t_exp_helper *expand,
    t_env *env, int exit_status);
char	*ft_itoa(int n);
int	is_valid_var_char(char c);
int	expand_fill(t_exp_helper *expand, t_token *tmp);
char	*lookup_variable(char *var_name, t_env *env_struct);
    
    t_cmd *parser(t_token *token_list);
    int is_redirection(char *str, int i);
    char **ft_split_q(char const *s, char c);
    void process_quotes_for_cmd(t_cmd *cmd_list, int remove_mode);
    
    
    
//add back func
void	add_cmd_back(t_cmd **lst, t_cmd *new);
void	add_redir_back(t_redir **lst, t_redir *new);
void	ft_lstadd_back_env(t_env **lst, t_env *new);
void	ft_lstadd_back_token(t_token **lst, t_token *new);
    
///free func
char	**free_split(char **split, size_t j);
void free_env_struct(t_env *env_struct);
void free_token_list(t_token *token_list);
//void free_string_array(char **array, int count);
void free_cmd_list(t_cmd *cmd_list);
void free_split_str(char **split_str);
char **free_split_q(char **split, size_t j);

///debugging tools
void print_cmd(t_cmd *cmd_list);
void	print_tokens(t_token *token_list);  ///// remove if the parsing finish
void print_env(t_env *env_struct); // remove after the parsing finish

void process_redir_helper(char str, int *quote_state);    
char *redir_extracter(char *str);

char	*ft_strjoin(char const *s1, char const *s2);
t_redir *creat_redir_list(char *str);
t_redir *creat_redir_list_helper0(char *str1, char *str2);
int creat_redir_list_helper(char *str);
static char *process_redir(char *str, int *pos);
t_redir *creat_redir_node(int type, char *file);



int	expand_fill_str(t_exp_helper *expand, char *str);
void print_ambiguous_redir_errors(t_cmd *cmd);
void ambiguous_finder(t_cmd *cmd);

void	process_string(char *str, t_exp_helper *expand,
					t_env *env, int exit_status);


int open_file(int type, char *file);
#endif
    
    
    
    