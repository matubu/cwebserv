#include <pthread.h>
#include "web_server.h"

#define THREADS_COUNT 1

int main()
{
	pthread_t threads[THREADS_COUNT];

	if (pthread_create(&threads[0], NULL, init_server, NULL))
		tprint(2, "Error creating thread for server");
	pthread_exit(NULL);
}
