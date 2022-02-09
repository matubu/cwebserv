#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#ifdef __APPLE__
# include <sys/socket.h>
#else
# include <sys/sendfile.h>
#endif

#define SEND_BUF 8192

void	print(int fd, const char *buf)
{
	write(fd, buf, strlen(buf));
}

void	print_uint(int fd, unsigned int n)
{
	int i;
	char buf[11];

	i = 11;
	buf[--i] = n % 10 + '0';
	while (n /= 10)
		buf[--i] = n % 10 + '0';
	write(fd, buf + i, 11 - i);
}

void	print_int(int fd, int nb)
{
	unsigned int n = nb;

	if (nb < 0 && (n = -nb))
		write(fd, "-", 1);
	print_uint(fd, n);
}

void	print_file(int ofd, char *filename)
{
	int	fd = open(filename, O_RDONLY);
	long int off = 0;

	print(1, filename);
	print(1, "\n");
	print_int(1, fd);
#ifdef __APPLE__
	struct sf_hdtr	hdtr = { NULL, 0, NULL, 0 };
	while (sendfile(ofd, fd, &off, SEND_BUf, &hdtr, 0))
		;
#else
	while (sendfile(ofd, fd, &off, SEND_BUF))
		;
#endif
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
	write(fd, str - i, i);
}

int	endwith(char *a, char *b)
{
	int i = strlen(a);
	int j = strlen(b);
	if (j > i) return (0);
	while (j)
		if (a[--i] != b[--j])
			return (0);
	return (1);
}
