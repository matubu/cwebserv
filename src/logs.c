#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "utils.h"

int	logfile(char *filename, char *str)
{
	int		fd = open(filename, O_CREAT|O_APPEND|O_WRONLY, S_IRWXU);
	time_t now = time(NULL);
	struct tm *tm = localtime(&now);
	char *date = asctime(tm);

	if (fd == -1)
	{
		print(2, "Error writing in log file\n");
		return 1;
	}
	write(fd, date, 24);
	write(fd, ": ", 2);
	print(fd, str);
	write(fd, "\n", 1);
	close(fd);
	return 0;
}

int	loginfo(char *str)
{
	return logfile("logs/info.log", str);
}

int	logerror(char *str)
{
	return logfile("logs/errors.log", str);
}