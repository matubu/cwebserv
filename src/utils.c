#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>

#define BUFF_SIZE 256

int	len(const char *str)
{
	int len = 0;

	while (str[len])
		len++;
	return len;
}

void	print_char(char c)
{
	write(1, &c, 1);
}

void	print(int fd, const char *buff)
{
	int len = 0;

	while (buff[len])
		len++;
	write(fd, buff, len);
}

void	print_uint(int fd, unsigned int n)
{
	int i;
	char res[11];

	i = 11;
	res[--i] = n % 10 + '0';
	while (n /= 10)
		res[--i] = n % 10 + '0';
	write(fd, res + i, 11 - i);
}

void	print_int(int fd, int nb)
{
	unsigned int n = nb;

	if (nb < 0 && (n = -nb))
		write(fd, "-", 1);
	print_uint(fd, n);
}

void print_file(int ofd, char *filename)
{
	int	fd = open(filename, O_RDONLY);
	int ret;
	char buff[BUFF_SIZE];

	while ((ret = read(fd, buff, BUFF_SIZE)))
		write(ofd, buff, ret);
	close(fd);
}


//case "test %"
void	tprint(int fd, char *str, ...)
{
	va_list args;
	va_start(args, str);
	//unsigned int i = 0;

	while (*str)
	{
		if (*str++ != '%')
			write(fd, str-1, 1);
			//i++;
		else
		{
			//write(fd, str - 1 - i, i);
			//i = 0;
			switch (*str++)
			{
				case '%':
					write(1, "%", 1);
					break;
				case 'c':
					print_char(va_arg(args, int));
					break;
				case 'u':
					print_uint(fd, va_arg(args, unsigned int));
					break;
				case 'd':
				case 'i':
					print_int(fd, va_arg(args, int));
					break;
				case 's':
					print(fd, va_arg(args, char *));
					break;
				case 't':
					print_file(fd, va_arg(args, char *));
					break;
				default:
					write(1, str-2, 2);
					break;
			}
		}
	}
	//write(fd, str - i, i);
}

/**
 * same as strcmp
 */
int	strdiff(char *a, char *b)
{
	while (*a && *a == *b && (a++ && b++))
		;
	return (*a - *b);
}
//startwith