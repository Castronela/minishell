// #include <unistd.h>
// #include <stdarg.h>
// #include <limits.h>

// int	ft_printf(const char *, ...);
// int	ft_form(va_list, char);
// int	ft_putchar(char);
// int	ft_putstr(char *);
// int	ft_putdec(int);
// int	ft_puthex(unsigned int);

// int	ft_printf(const char * str, ...)
// {
// 	va_list	ap;
// 	int		ec;
// 	int		printed;

// 	if (!str)
// 		return (-1);
// 	if (!*str)
// 		return (0);

// 	va_start(ap, str);
// 	ec = 0;
// 	printed = 0;

// 	while (*str)
// 	{
// 		if (*str == '%')
// 		{
// 			str++;
// 			if (*str && *str == '%')
// 				ec = ft_putchar(*str);
// 			else if (*str && (*str == 's' || *str == 'd' || *str == 'x'))
// 				ec = ft_form(ap, *str);
// 		}
// 		else
// 			ec = ft_putchar(*str);
// 		if (ec < 0)
// 			return (ec);
// 		printed += ec;
// 		str++;
// 	}
// 	va_end(ap);
// 	return (printed);
// }

// int	ft_form(va_list ap, char c)
// {
// 	int	printed;

// 	printed = 0;
// 	if (c == 's')
// 		printed = ft_putstr(va_arg(ap, char *));
// 	else if (c == 'd')
// 		printed = ft_putdec(va_arg(ap, int));
// 	else if (c == 'x')
// 		printed = ft_puthex(va_arg(ap, unsigned int));

// 	return (printed);
// }

// int	ft_putchar(char c)
// {
// 	return (write(1, &c, 1));
// }

// int	ft_putstr(char *str)
// {
// 	int	i;
// 	int	ec;
// 	int	printed;

// 	if (!str)
// 		return (printed = write(1, "(null)", 6), printed);
// 	i = 0;
// 	ec = 0;
// 	printed = 0;
// 	while (str[i])
// 	{
// 		ec = write(1, str + i, 1);
// 		if (ec < 0)
// 			return (ec);
// 		printed += ec;
// 		i++;
// 	}
// 	return (printed);
// }

// int	ft_putdec(int n)
// {
// 	int		printed;
// 	int		ec;
// 	char	c;
// 	long	num;

// 	num = (long)n;
// 	printed = 0;
// 	// if (num >= 2147483648 || num <= -2147483649)
// 	// 	return (-1);									// !!
// 	if (num < 0)
// 	{
// 		ec = write(1, "-", 1);
// 		if (ec < 0)
// 			return (ec);
// 		num = -num;
// 		printed += ec;
// 	}
// 	if (num >= 10)
// 		printed += ft_putdec(num / 10);					// !!
// 	c = (num % 10) + '0';
// 	ec = write(1, &c, 1);
// 	if (ec < 0)
// 		return (ec);
// 	printed += ec;
// 	return (printed);
// }

// int	ft_puthex(unsigned int hex)
// {
// 	char	*hd = "0123456789abcdef";
// 	long	i;
// 	int		ec;
// 	int		printed;

// 	i = 0;
// 	printed = 0;
// 	if (hex < 0)										// !!
// 	{
// 		ec = write(1, "-", 1);
// 		if (ec < 0)
// 			return (ec);
// 		printed += ec;
// 	}
// 	if (hex >= 16)
// 		printed += ft_puthex(hex / 16);
// 	i = hex % 16;
// 	ec = write(1, hd + i, 1);
// 	if (ec < 0)
// 		return (ec);
// 	printed += ec;
// 	return (printed);
// }

// #include <stdio.h>

// int	main(void)
// {
// 	long	a;
// 	char	c;
// 	char	*str = "Hello hi how are you?";
// 	int		count;

// 	a = -2147483649;
// 	c = 'y';

// 	count = 0;

// 	count = ft_printf("str:	%s\n", str);
// 	ft_printf("		count:	%d\n", count);
// 	count = printf("str:	%s\n", str);
// 	ft_printf("		count:	%d\n", count);

// 	count = ft_printf("c:	%s\n", &c);
// 	ft_printf("		count:	%d\n", count);
// 	count = printf("c:	%s\n", &c);
// 	printf("		count:	%d\n", count);

// 	count = ft_printf("a:	%d\n", (int)a);
// 	ft_printf("		count:	%d\n", count);
// 	count = printf("a:	%d\n", (int)a);
// 	printf("		count:	%d\n", count);
	
// 	count = ft_printf("h:	%x\n", (unsigned int)a);
// 	ft_printf("		count:	%d\n", count);
// 	count = printf("h:	%x\n", (unsigned int)a);
// 	printf("		count:	%d\n", count);

// 	return (0);
// }


#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

static void ft_putstr(const char *s, int *count)
{
    if (!s)
        s = "(null)";
    while (*s)
    {
        write(1, s++, 1);
        (*count)++;
    }
}

static void ft_putnbr(int n, int *count)
{
    char c;

    if (n == -2147483648)
    {
        ft_putstr("-2147483648", count);
        return;
    }
    if (n < 0)
    {
        write(1, "-", 1);
        (*count)++;
        n = -n;
    }
    if (n > 9)
        ft_putnbr(n / 10, count);
    c = (n % 10) + '0';
    write(1, &c, 1);
    (*count)++;
}

static void ft_puthex(unsigned int n, int *count)
{
    char *hex = "0123456789abcdef";

    if (n >= 16)
        ft_puthex(n / 16, count);
    write(1, &hex[n % 16], 1);
    (*count)++;
}

int ft_printf(const char *format, ...) /* You can find all this function in the manual. "man va_arg", "man 2 va_arg", "man 3 va_arg" */
{
    va_list args;
    int count = 0;

    va_start(args, format);
    while (*format)
    {
        if (*format == '%' && *(format + 1))
        {
            format++;
            if (*format == 's')
                ft_putstr(va_arg(args, char *), &count);
            else if (*format == 'd')
                ft_putnbr(va_arg(args, int), &count);
            else if (*format == 'x')
                ft_puthex(va_arg(args, unsigned int), &count);
            else
            {
                write(1, format, 1);
                count++;
            }
        }
        else
        {
            write(1, format, 1);
            count++;
        }
        format++;
    }
    va_end(args);
    return count;
}
