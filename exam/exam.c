#include <unistd.h>
#include <stdarg.h>
#include <limits.h>

int	ft_printf(const char *, ...);
int	ft_form(va_list ap, char c);
int	ft_puchar(char c);
int	ft_putstr(char *str);
int	ft_putdec(long num);
int	ft_puthex(long hex);


int	ft_printf(const char * str, ...)
{
	va_list	ap;
	int		ec;
	int		printed;

	if (!str)
		return (-1);
	if (!*str)
		return (0);

	va_start(ap, str);
	ec = 0;
	printed = 0;

	while (*str)
	{
		if (*str == '%')
		{
			str++;
			if (*str && *str == '%')
				ec = ft_putchar(*str);
			else if (*str && (*str == 's' || *str == d || *str == 'x'))
				ec = ft_form(ap, *str);
		}
		else
			ec = putchar(*str);
		if (ec < 0)
			return (ec);
		printed += ec;
		str++;
	}
	va_end(ap);
	return (printed);
}

int	ft_form(va_list ap, char c)
{
	int	printed;

	printed = 0;
	if (c == 's')
		printed = ft_putstr(va_arg(ap, char *));
	else if (c == 'd')
		printed = ft_putdec(va_arg(ap, long));
	else if (c == 'x')
		printed = ft_puthex(va_arg(ap, long));

	return (printed);
}

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(char *str)
{
	int	i;
	int	ec;
	int	printed;

	i = 0;
	ec = 0;
	printed = 0;
	while (str[i])
	{
		ec = write(1, str + 1, 1);
		if (ec < 0)
			return (ec);
		printed += ec;
		i++;
	}
	return (printed);
}

int	ft_putdec(long num)
{
	int		printed;
	int		ec;
	char	c;

	printed = 0;
	if (num >= 2147483648 || num <= -2147483649)
		return (-1);									// !!
	if (num < 0)
	{
		ec = write(1, "-", 1);
		if (ec < 0)
			return (ec);
		num = -num;
		printed += ec;
	}
	if (num >= 10)
		printed += ft_putdec(num / 10);					// !!
	c = (num % 10) + '0';
	ec = write(1, &c, 1);
	if (ec < 0)
		return (ec);
	printed += ec;
	return (printed);
}

int	ft_puthex(long hex)
{
	char	*hd = "0123456789abcdef";
	long	i;
	int		ec;
	int		printed;

	i = 0;
	printed = 0;
	if (hex < 0)										// !!
	{
		ec = write(1, "-", 1);
		if (ec < 0)
			return (ec);
		printed += ec;
	}
	if (hex >= 16)
		printed += ft_puthex(hex / 16);
	i = hex % 16;
	ec = write(1, hd + i, 1);
	if (ec < 0)
		return (ec);
	printed += ec;
	return (printed);
}

// #include <stdio.h>
