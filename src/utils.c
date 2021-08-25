#include <unistd.h>

int	print(int fd, const char *buff)
{
	int len = 0;
	while (buff[len])
		len++;
	return write(fd, buff, len);
}