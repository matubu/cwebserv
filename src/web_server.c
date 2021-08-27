#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h> //gethostbyname
#include <fcntl.h> //open close
#include <unistd.h> //read write
#include <stdlib.h> //exit
#include <pthread.h>
#include "logs.h"
#include "utils.h"

#include <stdio.h>

#define BUFF_SIZE 1024
#define NOT_FOUND "views/404.html"

void	send_file(int ofd, char *filename, char *filetype)
{
  struct stat stats;
  if (!filename || stat(filename, &stats) == -1) {
		if (!strdiff("text/html", filetype) && stat(NOT_FOUND, &stats) == 0)
			tprint(ofd, "HTTP/1.1 404 Not Found\nContent-length: %d\nContent-Type: text/html\n\n%t", stats.st_size, NOT_FOUND);
		else
			print(ofd, "HTTP/1.1 404 Not Found\n\n");
		return;
	}
	tprint(ofd, "HTTP/1.1 200 OK\nContent-length: %d\nContent-Type: %s\n\n%t", stats.st_size, filetype, filename);
}

typedef struct s_request
{
	char *type;
	char *url;
	char *protocol;
} t_request;

//WARNING parse_request edit the request string
t_request parse_request(char *str)
{
	char *head = str;
	t_request request = {0,0,0};

	while (*str && *str != '\n' && *str != '\r')
		str++;
	*str = '\0';
	loginfo(head);
	request.type = head;
	while(*head >= 'A' && *head <= 'Z')
		head++;
	*head++ = '\0';
	request.url = head;
	while(*head && *head != ' ')
		head++;
	*head++ = '\0';
	request.protocol = head;
	return request;
}

void *init_server() {
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
	address.sin_port = htons(80);

	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) < 0)
		logerror("Error binding (you may not have the permision or a server may be already launch on the port 80)", 1);

	loginfo("Server sucessfully lauch on port 80");
	while (1) {
		if (listen(create_socket, 10) < 0)
			logerror("Error listening request", 1);

		if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0)
			logerror("Error accepting request", 1);

		recv(new_socket, buffer, BUFF_SIZE, 0);
		t_request request = parse_request(buffer);

		tprint(1, "{\n\ttype: \"%s\",\n\turl: \"%s\",\n\tsheme: \"%s\"\n}\n", request.type, request.url, request.protocol);
		if (!strdiff(request.type, "GET") && !strdiff(request.url, "/test.jpg"))
			send_file(new_socket, "views/test.jpg", "image/jpg");
		else
			send_file(new_socket, 0, "text/html");

		close(new_socket);
	}
  pthread_exit(NULL);
}