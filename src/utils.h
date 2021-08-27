#ifndef UTILS_H
# define UTILS_H

int		len(const char *str);
void	print(int fd, const char *buff);
void	print_int(int fd, int v);
void	tprint(int fd, char *str, ...);
int		strdiff(char *a, char *b);

#endif