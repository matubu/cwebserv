#include <pthread.h>
#include "web_server.h"
#include "logs.h"

#define THREADS_COUNT 1

int main()
{
	pthread_t threads[THREADS_COUNT];

	if (pthread_create(&threads[0], NULL, init_server, NULL))
		logerror("Error creating thread for server", 1);
  pthread_exit(NULL);
}