/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:46:06 by castronela        #+#    #+#             */
/*   Updated: 2025/01/21 16:38:33 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				get_next_token(t_shell *shell, size_t *index_cmd, char **token,
					const int do_complex);

static int		get_argument_end_index(t_shell *shell, size_t *index_cmd,
					const int do_complex);
static void		skip_quoted_str(t_shell *shell, const char *str, size_t *index);
static size_t	operator_length_at_index(const char *str, const size_t index);

/*
Returns allocated string of next token starting from index 'i_cmdline'
!Note: cmdline != NULL
return:
0 - continue normally
1 - free all cmd vars and restart main prompt
2 - free all vars and exit
*/
int	get_next_token(t_shell *shell, size_t *index_cmd, char **token,
		const int do_complex)
{
	size_t	operator_len;
	size_t	i_start;

	skip_whitespaces(shell->cmdline, index_cmd);
	i_start = (*index_cmd);
	operator_len = operator_length_at_index(shell->cmdline, *index_cmd);
	if (do_complex && operator_len > 0)
		(*index_cmd) += operator_len;
	else if (get_argument_end_index(shell, index_cmd, do_complex))
	{
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			exit_early(shell, NULL, ERRMSG_WRITE);
		return (1);
	}
	*token = NULL;
	if (*index_cmd != i_start)
	{
		*token = ft_substr(shell->cmdline, i_start, ((*index_cmd) - i_start));
		if (!(*token))
			return (2);
	}
	skip_whitespaces(shell->cmdline, index_cmd);
	return (0);
}

/*
0 - continue normally
1 - free all cmd vars and restart main prompt
*/
static int	get_argument_end_index(t_shell *shell, size_t *index_cmd,
		const int do_complex)
{
	int	ret_value;

	while (shell->cmdline[*index_cmd])
	{
		skip_quoted_str(shell, shell->cmdline, index_cmd);
		if (do_complex && !shell->cmdline[*index_cmd] && shell->open_qt)
		{
			ret_value = secondary_prompt(shell, 1);
			if (ret_value == 1)
				return (0);
			else if (ret_value == 2)
				return (1);
		}
		else if (is_whitesp(shell->cmdline[*index_cmd]))
			break ;
		else if (do_complex && operator_length_at_index(shell->cmdline,
				*index_cmd) > 0)
			break ;
		(*index_cmd)++;
	}
	return (0);
}

/*
Modifies 'index' to point to closing quote or null terminator,
	if no closing quote found
	- stores opening quote char in 'shell->open_qt',
		to be used later for syntax check
Note: call ONLY when 'str' at 'index' is an opening quote char
*/
static void	skip_quoted_str(t_shell *shell, const char *str, size_t *index)
{
	if (!is_quote(str[*index]) && !shell->open_qt)
		return ;
	if (!shell->open_qt)
		shell->open_qt = str[(*index)++];
	while (str[*index] && str[*index] != shell->open_qt)
		(*index)++;
	if (str[*index] == shell->open_qt)
		shell->open_qt = 0;
}

/*
Returns length of longest operator that matches 'str' from 'index' or 0,
	if no matches found
*/
static size_t	operator_length_at_index(const char *str, const size_t index)
{
	return (find_longest_match_length(&str[index], (const char *[]){RD_IN,
			RD_OUT, RD_OUT_A, RD_HD, CT_PIPE, CT_AND, CS_SMICOL, CS_NEWLNE,
			NULL}));
}
