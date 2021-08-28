#include <pthread.h>
#include "web_server.h"
#include "web_socket.h"
#include "logs.h"

#define THREADS_COUNT 1

//TODO not use exit to exit properly and destoy thread
int main()
{
	pthread_t threads[THREADS_COUNT];

	if (pthread_create(&threads[0], NULL, init_server, NULL))
		logerror("Error creating thread for server");
	// init_ws_server();
  pthread_exit(NULL);
}
