#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "logs.h"
#include "utils.h"

#define BUFF_SIZE 1024

void	send_file(int ofd, char *filename, char *filetype)
{
  struct stat sb;
  if (stat(filename, &sb) == -1) {
		print(ofd, "HTTP/1.1 404 Not Found\n\n");
		return;
	}
	tprint(ofd, "HTTP/1.1 200 OK\nContent-length: %d\nContent-Type: %s\n\n%t", sb.st_size, filetype, filename);
}

int main() {
	int									create_socket, new_socket;
	socklen_t						addrlen;
	char								buffer[BUFF_SIZE] = {0};
	struct sockaddr_in	address;

	//filedescriptor = socket(
	//	domain = AF_INET = IPv4 Internet protocols,
	//	type = SOCK_STREAM = two-way, connection-based byte streams,
	//	protocol = default
	//);
	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0)
		loginfo("Starting server...");

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);

	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) < 0)
		logerror("Error binding", 1);

	while (1) {
		if (listen(create_socket, 10) < 0)
			logerror("Error listening request", 1);

		if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0)
			logerror("Error accepting request", 1);

		if (new_socket > 0)
			loginfo("Connection");

		recv(new_socket, buffer, BUFF_SIZE, 0);
		loginfo(buffer);
		//send_file(new_socket, "src/index.html", "text/html");
		send_file(new_socket, "src/test.jpg", "image/jpg");
		close(new_socket);
	}
	return 0;
}