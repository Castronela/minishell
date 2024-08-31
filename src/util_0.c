#include "minishell.h"

/*
Returns true if c is whitespace
*/
bool	is_ws(const char c)
{
	if (c >= 9 && c <= 13)
		return (true);
	else if (c == 32 || c == 9)
		return (true);
	return (false);
}

/*
Returns true if c is operator
*/
bool	is_op(const char c)
{
	if (c == '<')
		return (true);
	else if (c == '>')
		return (true);
	else if (c == '|')
		return (true);
	return (false);
}

/*
Returns true if c is single or double quote
*/
bool	is_qt(const char c)
{
	if (c == '\"')
		return (true);
	else if (c == '\'')
		return (true);
	return (false);
}

/*
Removes closed quotes from token
*/
void	rm_qt(char *token)
{
	char	qt_open;
	size_t	i_token;
	size_t	len_token;

	qt_open = 0;
	i_token = 0;
	len_token = ft_strlen(token);
	while (i_token < len_token)
	{
		if (qt_open == 0 && is_qt(token[i_token]))
		{
			qt_open = token[i_token];
			ft_memmove(&token[i_token], &token[i_token + 1], --len_token);
			token[len_token] = 0;
			i_token--;
		}
		else if (qt_open == token[i_token])
		{
			qt_open = 0;
			ft_memmove(&token[i_token], &token[i_token + 1], --len_token);
			token[len_token] = 0;
			i_token--;
		}
		i_token++;
	}
}
