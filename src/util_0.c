#include "minishell.h"

bool	is_whitespace(const char c)
{
	if (c >= 9 && c <= 13)
		return (true);
	else if (c == 32)
		return (true);
	return (false);
}

bool	is_operator(const char c)
{
	if (c == '<')
		return (true);
	else if (c == '>')
		return (true);
	else if (c == '|')
		return (true);
	return (false);
}

bool	is_quote(const char c)
{
	if (c == '\"')
		return (true);
	else if (c == '\'')
		return (true);
	return (false);
}
