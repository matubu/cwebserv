#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "logs.h"
#include "utils.h"

#define BUFF_SIZE 1024

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

	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0)
		loginfo("Binding Socket");

	while (1) {
		if (listen(create_socket, 10) < 0) {
			logerror("server: listen");
			exit(1);
		}

		if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {
			logerror("server: accept");
			exit(1);
		}

		if (new_socket > 0)
			loginfo("The Client is connected...");

		recv(new_socket, buffer, BUFF_SIZE, 0);
		loginfo(buffer);
		print(new_socket, "HTTP/1.1 200 OK\n");
		print(new_socket, "Content-length: 46\n");
		print(new_socket, "Content-Type: text/html\n\n");
		print(new_socket, "<html><body><H1>Hello world</H1></body></html>");
		close(new_socket);
	}
	close(create_socket);
	return 0;
}