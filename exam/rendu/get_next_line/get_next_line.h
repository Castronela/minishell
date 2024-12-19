// #ifndef GET_NEXT_LINE_H
// # define GET_NEXT_LINE_H

// # include <unistd.h>
// # include <stdlib.h>
// # include <fcntl.h>

// # ifndef BUFFER_SIZE
// #  define BUFFER_SIZE 42
// # endif

// char	*get_next_line(int fd);

// size_t	line_length(char *str);
// char	*join_parts(char *s1, char *s2);
// char	*copy_n_shift(char *buffer);
// char	*extract_rest(char *next_line, char *buffer, int fd);

// #endif




#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>	
# include <stdio.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);

#endif