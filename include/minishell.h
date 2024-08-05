# ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/Libft/include/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <errno.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h>

#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

/*--- Type Definitions ---*/

typedef struct e_lsttoken
{
    char *token;
    struct e_lsttoken *next;
}   t_lsttoken;

typedef struct s_shell_data
{
    t_lsttoken **tokenlst;
    char *cmdline;
}   t_shell_data;

/* Function Prototypes */

int main();

/*--- Tokenizer ---*/

int         tokenizer(t_shell_data *shell);
char        *get_next_token(const char *cmdline, ssize_t *i_cmdline);

/*--- Token linked list*/

t_lsttoken  **tokenlst_memalloc(void);
int         tokenlst_addtoken(t_lsttoken **head, char *token);
void        tokenlst_memfreelist(t_lsttoken **head);

/*--- Utils ---*/

bool        is_whitespace(const char c);
bool        is_operator(const char c);
bool        is_quote(const char c);
bool        check_quote(const char c, ssize_t *i_cmdline, const char first_char);
bool        check_operator(const char c, const char first_char);
char        *extract_str(const char *src, ssize_t i_start, ssize_t i_end);

/*--- Testing ---*/

void        print_lst(t_lsttoken **head);

/* End Function Prototypes */

# endif 
