#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>

int n_putnbr_base(long long n, int base)
{
	char digits[]= "0123456789abcdef";
	int len;

	len = 0;
	if (n < 0)
	{
		n = n * -1;
		len += write(1,"-",1);
	}
	if (n/base != 0)
		len += n_putnbr_base(n/base, base);
	len += write(1,&(digits[n%base]),1);
	
	return (len);
}


int	ft_printf(const char *format, ...)
{
	int length;
	va_list ar;
	char *str;
	
	length = 0;
	va_start(ar, format);
	while(*format)
	{
		if (*format != '%')
		{
			length += write(1,format,1);
		}
		else
		{
			format++;
			if(*format == 'd')
				length += n_putnbr_base((long long)va_arg(ar, int), 10);
			else if(*format == 'x')
				length += n_putnbr_base((long long)va_arg(ar, int), 16);
			else if(*format == 's')
			{
				str = va_arg(ar, char *);
				if(!str)
					str = "(null)";
				while(*str)
				{
					length += write(1, str, 1);
					str++;
				}
			}
			else if(*format == '%')
				length += write(1, "%", 1);
		}
		format++;
	}
	va_end(ar);
	return(length);
}

/*
#include <stdlib.h>

int main(int argc, char **argv)
{
	(void)argc;

	(void)argv;

	int length_original = printf("%s",argv[1]);
	printf(":-length_original:%d\n",length_original);
	fflush(NULL);
	int length_tuyo = ft_printf("%s",argv[1]);
	printf(":-length_tuyo:%d\n",length_tuyo);


	int length_original = printf("hola");
	printf(":-length_original:%d\n",length_original);
	fflush(NULL);
	int length_tuyo = ft_printf("hola");
	printf(":-length_tuyo:%d\n",length_tuyo);
	fflush(NULL);

	printf("\n");

	length_original = printf("%dcero%d%d%d", 0,1,100,999);
	printf(":-length_original:%d\n",length_original);
	fflush(NULL);
	length_tuyo = ft_printf("%dcero%d%d%d",0,1,100,999);
	printf(":-length_tuyo:%d\n",length_tuyo);
	fflush(NULL);

	length_original= printf("%%%s%x","hola", 765675);
	printf(":-length_original:%d\n",length_original);
	fflush(NULL);
	length_tuyo = ft_printf("%%%s%x","hola", 765675);
	printf(":-length_tuyo:%d\n",length_tuyo);
	fflush(NULL);

	printf("\n");



	return(0);

}
*/

