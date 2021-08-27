#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include "utils.h"

#define INFO_LOG "logs/info.log"
#define ERRORS_LOG "logs/errors.log"

void	logfile(char *filename, char *str)
{
	int		fd = open(filename, O_CREAT|O_APPEND|O_WRONLY, S_IRWXU);
	time_t now = time(NULL);
	struct tm *tm = localtime(&now);
	char *date = asctime(tm);

	if (fd == -1)
		print(2, "Error writing in log file\n");
	date[24] = '\0';
	tprint(fd, "%s: %s\n", date, str);
	close(fd);
}

void	loginfo(char *str)
{
	logfile(INFO_LOG, str);
}

void	logerror(char *str, int status)
{
	logfile(ERRORS_LOG, str);
	if (!status)
		return;
	tprint(2, "Exiting with status code %d: \"%s\"\n", status, str);
	logfile(ERRORS_LOG, "Exiting program...");
	exit(status);
}