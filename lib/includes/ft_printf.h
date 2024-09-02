/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 16:21:10 by pamatya           #+#    #+#             */
/*   Updated: 2024/07/04 04:07:00 by pamatya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"

int		ft_printf(const char *str, ...);
int		ft_fprintf(int fd, const char *str, ...);
int		ft_form(int fd, va_list arg, char opt);
int		ft_putchr(int fd, unsigned char c);
int		ft_putstr(int fd, char *str);
int		ft_putdec(int fd, long num);
int		ft_puthex(int fd, unsigned int hexnum, char opt);
int		ft_putptr(int fd, void *address);
void	ft_revstr(char *str);
void	ft_strtoup(char *str);
void	ft_basetostr(long num_arg, char *str, long base);
int		ft_intlen(long n, int base);
int		ft_lulen(unsigned long n, int base);

#endif
