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

int	print(int fd, const char *buff)
{
	int len = 0;
	while (buff[len])
		len++;
	return write(fd, buff, len);
}

void	print_int(int fd, int v)
{
	int i;
	unsigned int n;
	char res[12];

	i = 12;
	n = (unsigned int)v;
	res[--i] = n % 10 + '0';
	while (n /= 10)
		res[--i] = n % 10 + '0';
	if (v < 0)
		res[--i] = '-';
	write(fd, res + i, 12 - i);
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

void	tprint(int fd, char *str, ...)
{
	va_list args;
	va_start(args, str);
	unsigned int i = 0;

	while (*str)
	{
		if (*str++ != '%')
			i++;
		else
		{
			write(fd, str - 1 - i, i);
			i = 0;
			switch (*str++)
			{
				case '%':
					write(1, "%", 1);
					break;
				case 'd':
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
	write(fd, str - i, i);
}