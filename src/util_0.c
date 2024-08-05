#include "minishell.h"

bool    is_whitespace(const char c)
{
    if (c >= 9 && c <= 13)
        return (true);
    else if (c == 32)
        return (true);
    return (false);
}

bool    is_operator(const char c)
{
    if (c == '<')
        return (true);
    else if (c == '>')
        return (true);
    else if (c == '|')
        return (true);
    return (false);
}

bool    is_quote(const char c)
{
    if (c == '\"')
        return (true);
    else if (c == '\'')
        return (true);
    return (false);
}

/*
Returns true if first_char is non-quote and c is quote
OR
if first_char is quote and same type as c (also increments i_cmdline)
*/
bool    check_quote(const char c, ssize_t *i_cmdline, const char first_char)
{
    if (is_quote(first_char) == false)
    {
        if (is_quote(c))
            return (true);
    }
    else
    {
        if (c == first_char)
        {
            (*i_cmdline)++;
            return (true);
        }
    }
    return (false);
}

/*
Returns true if first_char is non-quote and non-operator and c is operator 
OR
if first_char is operator and different than c
*/
bool    check_operator(const char c, const char first_char)
{
    if (is_operator(first_char) == false)
    {
        if (is_operator(c) && is_quote(first_char) == false)
            return (true);
    }
    else
    {
        if (c != first_char)
            return (true);
    }
    return (false);
}


/*
Returns allocated pointer with copy of src from i_start to i_end
*/
char *extract_str(const char *src, ssize_t i_start, ssize_t i_end)
{
    ssize_t diff;
    char *str;

    if (src == NULL)
        return (NULL);
    diff = i_end - i_start;
    str = malloc((diff + 1) * sizeof(char));
    if (str == NULL)
        perror("Error malloc");
    else
    {
        *str = 0;
        ft_strlcpy(str, &src[i_start], diff + 1);
    }
    return (str);
}
