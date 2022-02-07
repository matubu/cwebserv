#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "utils.h"

void	log(char *str)
{
	time_t now = time(NULL);
	struct tm *tm = localtime(&now);
	char *date = asctime(tm);

	date[24] = '\0';
	tprint(2, "%s: %s\n", date, str);
}
