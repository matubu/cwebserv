#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> //open close
#include <unistd.h> //read write
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "utils.h"

#define RECV_BUF 1024
#define PATH_BUF 1024
#define VIEWS "./views/"
#define PORT 8080

typedef struct {
	char *ext;
	char *mime;
}	filetype_t;

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

/**
 * return 1 if the path is safe
 */
int	safe_path(char *str)
{
	int i = -1;

	if (strlen(str) > PATH_BUF - strlen(VIEWS))
		return (0);
	while (str[++i])
		if (str[i] == '.' && str[i + 1] == '.')
			return 0;
	return 1;
}

void	sendf(int ofd, char *filename)
{
	struct stat	stats;
	char	path[PATH_BUF + 1] = VIEWS;
	int		i = -1;

	if (safe_path(filename))
	{
		strcat(path, filename);
		while (filetypes[++i].ext)
		{
			if (endwith(filename, filetypes[i].ext))
			{
				if (stat(path, &stats) == -1)
					break ;
				tprint(ofd, "HTTP/1.1 200 OK\nContent-length: %d\nContent-Type: %s\n\n%t", stats.st_size, filetypes[i].mime, path);
				return ;
			}
		}
	}
	print(ofd, "HTTP/1.1 404 Not Found\n\n");
}

typedef struct
{
	char *type;
	char *url;
	char *protocol;
}	t_request;

/*
 * edit the request string to create a struct from it
 */
t_request	parse_request(char *str)
{
	char		*head = str;
	t_request	request = { NULL, NULL, NULL };

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
	tprint(1, "%s(\"%s\", %s)\n", request.type, request.url, request.protocol);
	return request;
}

int	main() {
	int					sock, new_socket;
	socklen_t			addrlen;
	char				buf[RECV_BUF + 1] = {0};
	struct sockaddr_in	addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(PORT)
	};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0
		|| bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0
		|| listen(sock, 10) < 0)
	{
		tprint(2, "Error starting server on the port %d\n", PORT);
		return (1);
	}

	tprint(2, "Server successfully launch on port %d\n", PORT);

	while (1) {
		if ((new_socket = accept(sock, (struct sockaddr *) &addr, &addrlen)) < 0)
		{
			tprint(2, "Error accepting new request");
			continue ;
		}

		int	ret = recv(new_socket, buf, RECV_BUF, 0);
		if (ret < 0) continue ; // need to close new_socket ?
		buf[ret] = '\0';

		t_request request = parse_request(buf);
		sendf(new_socket, request.url);
		close(new_socket);
	}
	return (0);
}
