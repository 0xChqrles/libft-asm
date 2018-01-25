#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>

void	ft_cat(int fd);

int main(int ac, char **av) {
	char		*ptr;
	int			len;
	int			ret;
	char		buf[len];
	char		*cmd;
	int			fd;
	struct stat	sbuf;

	if ((fd = open(av[0], O_RDONLY)) < 1 || fstat(fd, &sbuf) < 0)
		exit(EXIT_FAILURE);
	if ((ptr = mmap(0, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		exit(EXIT_FAILURE);
	ft_cat(fd);
	munmap(ptr, sbuf.st_size);
}
