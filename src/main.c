#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> //open close
#include <unistd.h> //read write
#include <pthread.h>
#include <string.h>
#include "utils.h"

#define RECV_BUF 1024
#define PATH_BUF 1024
#define VIEWS "views/"
#define NOT_FOUND "views/404.html"
#define PORT 8080

typedef struct {
	char *ext;
	char *mime;
} filetype_t;

const filetype_t filetypes[] = {
	{ ".html", "text/html" },
	{ ".txt", "text/plain" },
	{ ".jpg", "image/jpg" },
	{ ".jpeg", "image/jpeg" },
	{ ".mp4", "video/mp4" },
	{ ".gif", "image/gif" },
	{ ".ico", "image/ico" },
	{ ".css", "text/css" },
	{ ".js", "text/javascript" },
	{ 0 }
};

void	send_file(int ofd, char *path, char *filetype)
{
	struct stat stats;

	if (path && stat(path, &stats) != -1)
		tprint(ofd, "HTTP/1.1 200 OK\nContent-length: %d\nContent-Type: %s\n\n%t", stats.st_size, filetype, path);
	else if (!strdiff("text/html", filetype) && stat(NOT_FOUND, &stats) == 0)
		tprint(ofd, "HTTP/1.1 404 Not Found\nContent-length: %d\nContent-Type: text/html\n\n%t", stats.st_size, NOT_FOUND);
	else
		print(ofd, "HTTP/1.1 404 Not Found\n\n");
}

void sendf(int ofd, char *filename)
{
	char path[PATH_BUF + 1] = VIEWS;
	char *filetype = 0;
	int i = -1;

	while (filetypes[++i].ext)
	{
		if (endwith(filetypes[i].ext, filename) && (filetype = filetypes[i].mime))
		{
			send_file(ofd, strcat(path, filename), filetype);
			return ;
		}
	}
	send_file(ofd, 0, "text/html");
}

typedef struct s_request
{
	char *type;
	char *url;
	char *protocol;
} t_request;

/*
 * edit the request string to create a struct from it
 */
t_request parse_request(char *str)
{
	char		*head = str;
	t_request	request = {0,0,0};

	while (*str && *str != '\n' && *str != '\r')
		str++;
	*str = '\0';
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

/**
 * return 1 if the path is safe
 */
int safe_path(char *str)
{
	int i = -1;

	if (len(str) > PATH_BUF - len(VIEWS))
		return (0);
	while (str[++i])
		if (str[i] == '.' && (i == 0 || str[i - 1] == '/' || str[i - 1] == '\\'))
			return 0;
	return 1;
}

int main() {
	int					sock, new_socket;
	socklen_t			addrlen;
	char				buf[BUF_SIZE + 1] = {0};
	struct sockaddr_in	addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(PORT)
	};
	int ret;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0
		|| bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0
		|| listen(sock, 10) < 0)
	{
		tprint(2, "Error stating server (you may not have the permision or a server may be already launch on the port %d)\n", PORT);
		return (1);
	}

	tprint(2, "Server sucessfully lauch on port %d\n", PORT);

	while (1) {
		if ((new_socket = accept(sock, (struct sockaddr *) &addr, &addrlen)) < 0)
		{
			tprint(2, "Error accepting new request");
			return (1);
		}

		ret = recv(new_socket, buf, RECV_BUF, 0);
		if (ret < 0) continue ; // need to close new_socket ?
		buf[ret] = '\0';

		t_request request = parse_request(buf);

		tprint(1, "%s(\"%s\", %s)\n", request.type, request.url, request.protocol);

		if (!strdiff(request.url, "/"))
			send_file(new_socket, "views/index.html", "text/html");
		else if (safe_path(request.url))
			sendf(new_socket, request.url);
		else
			send_file(new_socket, 0, "text/html");

		close(new_socket);
	}
	return (0);
}
