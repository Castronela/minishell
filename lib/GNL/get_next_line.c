/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:37:49 by pamatya           #+#    #+#             */
/*   Updated: 2024/06/13 13:07:03 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

char	*get_next_line(int fd);
size_t	line_length(char *str);
char	*join_parts(char *s1, char *s2);
char	*copy_n_shift(char *buffer);
char	*extract_rest(char *next_line, char *buffer, int fd);

char	*get_next_line(int fd)
{
	ssize_t		bytes_read;
	static char	buffer[4096][BUFFER_SIZE + 1];
	char		*next_line;
	char		*joined_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer[fd][0])
	{
		bytes_read = read(fd, &(buffer[fd][0]), BUFFER_SIZE);
		if (bytes_read <= 0)
			return (NULL);
		buffer[fd][bytes_read] = '\0';
	}
	next_line = copy_n_shift(&(buffer[fd][0]));
	if (!next_line)
		return (NULL);
	if (next_line[line_length(next_line) - 1] != '\n')
	{
		joined_line = extract_rest(next_line, &(buffer[fd][0]), fd);
		if (!joined_line)
			return (NULL);
		next_line = joined_line;
	}
	return (next_line);
}

// Function to get the length of the string terminated by \n or \0, whichever
// occurs first, also counting the \n character if it occurs within the count.
size_t	line_length(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (*str && *str != '\n')
	{
		i++;
		str++;
	}
	if (*str == '\n')
		i++;
	return (i);
}

// Function to join two strings by allocating with malloc
char	*join_parts(char *s1, char *s2)
{
	char	*joined_str;
	char	*str_ptr[2];
	size_t	str_len[2];
	size_t	len;
	size_t	i;

	str_len[0] = line_length(s1);
	str_len[1] = line_length(s2);
	len = str_len[0] + str_len[1];
	joined_str = (char *)malloc((len + 1) * sizeof(char));
	if (!joined_str)
		return (NULL);
	i = 0;
	str_ptr[0] = s1;
	str_ptr[1] = s2;
	while (i < str_len[0])
		joined_str[i++] = *s1++;
	while (i >= str_len[0] && i < len)
		joined_str[i++] = *s2++;
	joined_str[i] = '\0';
	return (free(str_ptr[0]), free(str_ptr[1]), joined_str);
}

// Function to write the contents of the buffer to a new string "store" until
// the \n character is found and shift the rest of the buffer storage to the
// beginning, or until the buffer is emptied before finding \n. Then it
// returns the new string store
char	*copy_n_shift(char *buffer)
{
	char	*line_part;
	char	*buf_ptr;
	int		index;
	int		line_len;

	if (!buffer)
		return (NULL);
	line_len = line_length(buffer);
	line_part = (char *)malloc((line_len + 1) * sizeof(char));
	if (!line_part)
		return (NULL);
	index = 0;
	buf_ptr = buffer;
	while (*buffer && index < line_len)
		line_part[index++] = *buffer++;
	line_part[index] = '\0';
	while (*buffer)
		*buf_ptr++ = *buffer++;
	*buf_ptr = *buffer;
	return (line_part);
}

// Function to extract the rest part of the next line with buffer as array
char	*extract_rest(char *next_line, char *buffer, int fd)
{
	ssize_t	bytes_read;
	char	*line_part;
	char	*joined_line;

	while (next_line[line_length(next_line) - 1] != '\n')
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(next_line), NULL);
		else if (bytes_read == 0)
			return (next_line);
		buffer[bytes_read] = '\0';
		line_part = copy_n_shift(buffer);
		if (!line_part)
			return (free(next_line), NULL);
		joined_line = join_parts(next_line, line_part);
		if (!joined_line)
			return (free(next_line), free(line_part), NULL);
		next_line = joined_line;
	}
	return (next_line);
}
