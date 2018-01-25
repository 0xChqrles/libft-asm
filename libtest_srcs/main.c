#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

// PART - 1

int		ft_puts(const char *s);
void	ft_bzero(void *s, size_t n);
char	*ft_strcat(void *dest, void *src);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);

// PART - 2

int		ft_strlen(const char *s);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *restrict dst, const void *restrict src, size_t n);
char	*ft_strdup(const char *s1);

// PART - 3

void	ft_cat(int fd);

// BONUS

char	*ft_strcpy(char * dst, const char * src);
void	*ft_strncpy(char * dst, const char * src, size_t len);
char	*ft_strchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strrchr(const char *s, int c);

typedef	struct			s_test
{
	char				*name;
	struct s_subtest	*subtests;
	struct s_test		*next;
}						t_test;

typedef	struct			s_subtest
{
	char				*explanation;
	int8_t				result;
	struct s_subtest	*next;
}						t_subtest;

char			*binary_name;
int				status;
pid_t			pid;

int				_stdout;
int				fildes[2];

t_test			*tests = NULL;
char			*g_res[7] = {
	"OK",
	"KO",
	"CRASH",
	"NO-CRASH",
	"BUS",
	"SEGV",
	"ABRT"
};

#define OK			0
#define KO			1
#define CRASH		2
#define NO_CRASH	3
#define BUS			4
#define SEGV		5
#define ABRT		6

#define COK			"\033[32;1m"
#define CKO			"\033[31;1m"
#define EOC			"\033[0m"

#define STDOUT		1
#define STDERR		2

#define S_1			"basic\n"
#define S_2			"you see me\0but not me\n"
#define S_NULL		"(null)"

#define SANDBOX(x)	if (!(pid = fork())) { x; exit(EXIT_SUCCESS); } else { wait(&status); }

t_subtest	*new_subtest(char *explanation, int8_t result)
{
	t_subtest	*new;

	if (!(new = malloc(sizeof(t_subtest))))
		return (NULL);
	new->next = NULL;
	new->explanation = strdup(explanation);
	new->result = result;
	return (new);
}

t_test		*new_test(char *name)
{
	t_test	*new;

	if (!(new = malloc(sizeof(t_test))))
		return (NULL);
	new->next = NULL;
	new->subtests = NULL;
	new->name = strdup(name);
	return (new);
}

void		add_test(t_test **tests, char *name)
{
	t_test	*tmp;

	tmp = *tests;
	if (!tmp)
	{
		*tests = new_test(name);
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_test(name);
}

void		add_subtest(t_subtest **subtests, char *explanation, int8_t result)
{
	t_subtest	*tmp;

	tmp = *subtests;
	if (!tmp)
	{
		*subtests = new_subtest(explanation, result);
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_subtest(explanation, result);
}

void	fd_to_buf(int fd)
{
	_stdout = dup(fd);
	if (pipe(fildes))
		return ;
	dup2(fildes[1], fd);
	close(fildes[1]);
}

char	handle_status(int expect)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == expect)
			return (OK);
		else if (sig == SIGSEGV)
			return (SEGV);
		else if (sig == SIGBUS)
			return (BUS);
		else if (sig == SIGABRT)
			return (ABRT);
		else
			return (CRASH);
	}
	else if (expect)
		return (NO_CRASH);
	else if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		return (OK);
	else
		return (KO);
}

void	display_results(t_test *tests)
{
	t_subtest	*tmp;

printf("\033[36;1m\n\n \
	_________________ ____________\n \
	___  /___(_)__  /____  __/_  /_       ______ ______________ ___\n \
	__  / __  /__  __ \\_  /_ _  __/       _  __ `/_  ___/_  __ `__ \\\n \
	_  /___  / _  /_/ /  __/ / /_         / /_/ /_(__  )_  / / / / /\n \
	/_____/_/  /_.___//_/    \\__/         \\__,_/ /____/ /_/ /_/ /_/\n\n\n%s", EOC);
	while (tests)
	{
		tmp = tests->subtests;
		printf("%s:", tests->name);
		while (tmp)
		{
			printf(" %s[%s]%s", tmp->result ? CKO : COK, g_res[tmp->result], EOC);
			tmp = tmp->next;
		}
		tmp = tests->subtests;
		printf("\n");
		while (tmp)
		{
			if (tmp->result)
				printf("    %s[%s]%s %s\n", CKO, g_res[tmp->result], EOC, tmp->explanation);
			tmp = tmp->next;
		}
		tests = tests->next;
	}
}

t_subtest	**get_last_subtest(t_test **tests)
{
	t_test	*tmp;

	tmp = *tests;
	while (tmp->next)
		tmp = tmp->next;
	return (&tmp->subtests);
}

///////////////////////////////
//         ft_bzero          //
///////////////////////////////

void	ft_bzero_basic(t_subtest **subtests)
{
	SANDBOX(
		int		len = strlen(S_1);
		char	*s1 = ft_strdup(S_1);
		char	*s2 = strdup(S_1);

		ft_bzero(s1, len);
		bzero(s2, len);
		if (memcmp(s1, s2, len))
			exit(EXIT_FAILURE);
		free(s1);
		free(s2);
	);
	add_subtest(subtests, "Your ft_bzero doesn't work with basic input", handle_status(0));
}

void	ft_bzero_null(t_subtest **subtests)
{
	SANDBOX(
		ft_bzero(NULL, 42);
	);
	add_subtest(subtests, "Your ft_bzero doesn't segfault with null parameter", handle_status(SIGSEGV));
}

void	ft_bzero_overflow(t_subtest **subtests)
{
	SANDBOX(
		void	*s = malloc(1);

		ft_bzero(s, -1);
	);
	add_subtest(subtests, "Your ft_bzero doesn't segfault when the second parameter is too big", handle_status(SIGSEGV));
}

void	bzero_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_bzero");
	subtests = get_last_subtest(tests);
	ft_bzero_basic(subtests);
	ft_bzero_null(subtests);
	ft_bzero_overflow(subtests);
}

///////////////////////////////
//         ft_strcat         //
///////////////////////////////

void	ft_strcat_basic(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(10);
		char	*s2 = malloc(10);

		bzero(s1, 10);
		bzero(s2, 10);
		s1 = memcpy(s1, "test", 4);
		s2 = memcpy(s2, "test", 4);
		s1 = strcat(s1, "test\0");
		s2 = ft_strcat(s2, "test\0");
		if (memcmp(s1, s2, 10))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strcat doesn't work with basic input", handle_status(0));
}

void	ft_strcat_first_empty(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(10);
		char	*s2 = malloc(10);

		bzero(s1, 10);
		bzero(s2, 10);
		s1 = strcat(s1, "test\0");
		s2 = ft_strcat(s2, "test\0");
		if (memcmp(s1, s2, 10))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strcat doesn't work with an empty destination", handle_status(0));
}

void	ft_strcat_second_empty(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(10);
		char	*s2 = malloc(10);

		bzero(s1, 10);
		bzero(s2, 10);
		s1 = memcpy(s1, "test", 4);
		s2 = memcpy(s2, "test", 4);
		s1 = strcat(s1, "");
		s2 = ft_strcat(s2, "");
		if (memcmp(s1, s2, 10))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strcat doesn't work with an empty source", handle_status(0));
}

void	ft_strcat_both_empty(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(10);
		char	*s2 = malloc(10);

		bzero(s1, 10);
		bzero(s2, 10);
		s1 = memcpy(s1, "test", 4);
		s2 = memcpy(s2, "test", 4);
		s1 = strcat(s1, "");
		s2 = ft_strcat(s2, "");
		if (memcmp(s1, s2, 10))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strcat doesn't work with empty parameters", handle_status(0));
}

void	ft_strcat_first_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strcat(NULL, "\0");
	);
	add_subtest(subtests, "Your ft_strcat doesn't work with an empty destination", handle_status(SIGSEGV));
}

void	ft_strcat_second_null(t_subtest **subtests)
{
	SANDBOX(
		char	*s = malloc(1);

		s[0] = 0;
		ft_strcat(s, NULL);
	);
	add_subtest(subtests, "Your ft_strcat doesn't work with an empty source", handle_status(SIGSEGV));
}

void	ft_strcat_both_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strcat(NULL, NULL);
	);
	add_subtest(subtests, "Your ft_strcat doesn't work with null parameters", handle_status(SIGSEGV));
}

void	strcat_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_strcat");
	subtests = get_last_subtest(tests);
	ft_strcat_basic(subtests);
	ft_strcat_first_empty(subtests);
	ft_strcat_second_empty(subtests);
	ft_strcat_both_empty(subtests);
	ft_strcat_first_null(subtests);
	ft_strcat_second_null(subtests);
	ft_strcat_both_null(subtests);
}

///////////////////////////////
//        ft_isalpha         //
///////////////////////////////

void	ft_isalpha_basic(t_subtest **subtests)
{
	SANDBOX(
		for (int i = -200; i < 200; i++)
			if (ft_isalpha(i) != isalpha(i))
				exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_isalpha doesn't work with basic input", handle_status(0));
}


void	isalpha_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_isalpha");
	subtests = get_last_subtest(tests);
	ft_isalpha_basic(subtests);
}

///////////////////////////////
//        ft_isdigit         //
///////////////////////////////

void	ft_isdigit_basic(t_subtest **subtests)
{
	SANDBOX(
		for (int i = -200; i < 200; i++)
			if (ft_isdigit(i) != isdigit(i))
				exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_isdigit doesn't work with basic input", handle_status(0));
}


void	isdigit_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_isdigit");
	subtests = get_last_subtest(tests);
	ft_isdigit_basic(subtests);
}

///////////////////////////////
//        ft_isalnum         //
///////////////////////////////

void	ft_isalnum_basic(t_subtest **subtests)
{
	SANDBOX(
		for (int i = -200; i < 200; i++)
			if (ft_isalnum(i) != isalnum(i))
				exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_isalnum doesn't work with basic input", handle_status(0));
}


void	isalnum_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_isalnum");
	subtests = get_last_subtest(tests);
	ft_isalnum_basic(subtests);
}

///////////////////////////////
//        ft_isascii         //
///////////////////////////////

void	ft_isascii_basic(t_subtest **subtests)
{
	SANDBOX(
		for (int i = -200; i < 200; i++)
			if (ft_isascii(i) != isascii(i))
				exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_isascii doesn't work with basic input", handle_status(0));
}


void	isascii_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_isalpha");
	subtests = get_last_subtest(tests);
	ft_isascii_basic(subtests);
}

///////////////////////////////
//        ft_isprint         //
///////////////////////////////

void	ft_isprint_basic(t_subtest **subtests)
{
	SANDBOX(
		for (int i = -200; i < 200; i++)
			if (ft_isprint(i) != isprint(i))
				exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_isprint doesn't work with basic input an empty destination", handle_status(0));
}


void	isprint_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_isprint");
	subtests = get_last_subtest(tests);
	ft_isprint_basic(subtests);
}

///////////////////////////////
//        ft_toupper         //
///////////////////////////////

void	ft_toupper_basic(t_subtest **subtests)
{
	SANDBOX(
		for (int i = -200; i < 200; i++)
			if (ft_toupper(i) != toupper(i))
				exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_toupper doesn't work with basic input", handle_status(0));
}


void	toupper_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_toupper");
	subtests = get_last_subtest(tests);
	ft_toupper_basic(subtests);
}

///////////////////////////////
//        ft_tolower         //
///////////////////////////////

void	ft_tolower_basic(t_subtest **subtests)
{
	SANDBOX(
		for (int i = -200; i < 200; i++)
			if (ft_tolower(i) != tolower(i))
				exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_tolower doesn't work with basic input", handle_status(0));
}


void	tolower_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_tolower");
	subtests = get_last_subtest(tests);
	ft_tolower_basic(subtests);
}

///////////////////////////////
//          ft_puts          //
///////////////////////////////

void	ft_puts_basic(t_subtest **subtests)
{
	SANDBOX(
 		int		len = strlen(S_1) + 1;
		char	buf[len];
		int		ret;
		int		rax;

		fd_to_buf(STDOUT);
		rax = ft_puts(S_1);
		ret = read(fildes[0], &buf, len + 1);
		if (rax != 10 || ret != len || strncmp(buf, S_1, len - 1) || buf[ret - 1] != '\n')
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_puts doesn't work with basic input", handle_status(0));
}

void	ft_puts_detect_end(t_subtest **subtests)
{
	SANDBOX(
		int		len = strlen(S_2) + 1;
		char	buf[len];
		int		ret;
		int		rax;

		fd_to_buf(STDOUT);
		rax = ft_puts(S_2);
		ret = read(fildes[0], &buf, len + 1);
		if (rax != 10 || ret != len || strncmp(buf, S_2, len - 1) || buf[ret - 1] != '\n')
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_puts doesn't work with characters after the '\\0'", handle_status(0));
}

void	ft_puts_null(t_subtest **subtests)
{
	SANDBOX(
		int		len = strlen(S_NULL) + 1;
		char	buf[len];
		int		ret;
		int		rax;

		fd_to_buf(STDOUT);
		rax = ft_puts(NULL);
		ret = read(fildes[0], &buf, len + 1);
		if (rax != 10 || ret != len || strncmp(buf, S_NULL, len - 1) || buf[ret - 1] != '\n')
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_puts doesn't work with null parameter", handle_status(0));
}

void	puts_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_puts");
	subtests = get_last_subtest(tests);
	ft_puts_basic(subtests);
	ft_puts_detect_end(subtests);
	ft_puts_null(subtests);
}

///////////////////////////////
//         ft_strlen         //
///////////////////////////////

void	ft_strlen_basic(t_subtest **subtests)
{
	SANDBOX(
		if (ft_strlen(S_1) != strlen(S_1))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strlen doesn't work with basic input", handle_status(0));
}

void	ft_strlen_detect_end(t_subtest **subtests)
{
	SANDBOX(
		if (ft_strlen(S_2) != strlen(S_2))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strlen doesn't work with characters after the '\\0'", handle_status(0));
}

void	ft_strlen_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strlen(NULL);
	);
	add_subtest(subtests, "Your ft_strlen doesn't segfault with null parameter", handle_status(SIGSEGV));
}

void	strlen_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_strlen");
	subtests = get_last_subtest(tests);
	ft_strlen_basic(subtests);
	ft_strlen_detect_end(subtests);
	ft_strlen_null(subtests);
}

///////////////////////////////
//         ft_memset         //
///////////////////////////////

void	ft_memset_basic(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(10);
		char	*s2 = malloc(10);

		s1 = memcpy(s1, "0123456789", 10);
		s2 = memcpy(s2, "0123456789", 10);
		memset(s1, 0x55, 6);
		memset(s2, 0x55, 6);
		if (memcmp(s1, s2, 10))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_memset doesn't work with basic input", handle_status(0));
}

void	ft_memset_null(t_subtest **subtests)
{
	SANDBOX(
		ft_memset(NULL, 0, 42);
	);
	add_subtest(subtests, "Your ft_memset doesn't segfault with null parameter", handle_status(SIGSEGV));
}

void	ft_memset_zero(t_subtest **subtests)
{
	SANDBOX(
		ft_memset(NULL, 0, 0);
	);
	add_subtest(subtests, "Your ft_memset doesn't work with zero as len parameter", handle_status(0));
}

void	memset_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_memset");
	subtests = get_last_subtest(tests);
	ft_memset_basic(subtests);
	ft_memset_null(subtests);
	ft_memset_zero(subtests);
}

///////////////////////////////
//         ft_memcpy         //
///////////////////////////////

void	ft_memcpy_basic(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(10);
		char	*s2 = malloc(10);

		s1 = memset(s1, 0x55, 10);
		s2 = memset(s2, 0x55, 10);
		s1 = memcpy(s1, "Hey ! ! ! ! !", 3);
		s2 = ft_memcpy(s2, "Hey ! ! ! ! !", 3);
		if (memcmp(s1, s2, 10))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_memcpy doesn't work with basic input", handle_status(0));
}

void	ft_memcpy_first_null(t_subtest **subtests)
{
	SANDBOX(
		ft_memcpy(NULL, ":)", 2);
	);
	add_subtest(subtests, "Your ft_memcpy doesn't segfault with a null destination", handle_status(SIGSEGV));
}

void	ft_memcpy_second_null(t_subtest **subtests)
{
	SANDBOX(
		ft_memcpy(malloc(1), NULL, 2);
	);
	add_subtest(subtests, "Your ft_memcpy doesn't segfault with a null source", handle_status(SIGSEGV));
}

void	ft_memcpy_zero(t_subtest **subtests)
{
	SANDBOX(
		ft_memcpy(NULL, NULL, 0);
	);
	add_subtest(subtests, "Your ft_memcpy doesn't work with a zero len", handle_status(0));
}

void	memcpy_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_memcpy");
	subtests = get_last_subtest(tests);
	ft_memcpy_basic(subtests);
	ft_memcpy_first_null(subtests);
	ft_memcpy_second_null(subtests);
	ft_memcpy_zero(subtests);
}

///////////////////////////////
//         ft_strdup         //
///////////////////////////////

void	ft_strdup_basic(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = strdup(S_1);
		char	*s2 = ft_strdup(S_1);

		if (memcmp(s1, s2, strlen(S_1) + 1))
			exit(EXIT_FAILURE);
		free(s1);
		free(s2);
	);
	add_subtest(subtests, "Your ft_strdup doesn't work with basic input", handle_status(0));
}

void	ft_strdup_detect_end(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(30);
		char	*s2 = malloc(30);
		char	*sd1 = strdup(S_2);
		char	*sd2 = ft_strdup(S_2);

		s1 = memset(s1, 0x55, 20);
		s2 = memset(s2, 0x55, 20);
		s1[2] = 0;
		s2[2] = 0;
		s1 = strncat(s1, sd1, 25);
		s2 = strncat(s2, sd2, 25);
		if (memcmp(s1, s2, 20))
			exit(EXIT_FAILURE);
		free(sd1);
		free(sd2);
	);
	add_subtest(subtests, "Your ft_strdup doesn't work with characters after the '\\0'", handle_status(0));
}

void	ft_strdup_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strdup(NULL);
	);
	add_subtest(subtests, "Your ft_strdup doesn't segfault with a null parameter", handle_status(SIGSEGV));
}

void	strdup_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_strdup");
	subtests = get_last_subtest(tests);
	ft_strdup_basic(subtests);
	ft_strdup_detect_end(subtests);
	ft_strdup_null(subtests);
}

///////////////////////////////
//          ft_cat           //
///////////////////////////////

void	ft_cat_basic(t_subtest **subtests)
{
	SANDBOX(
		char		*ptr;
 		int			len;
		int			ret;
		char		buf[len];
		char		*cmd;
		int			fd;
		struct stat	sbuf;

		if ((fd = open(binary_name, O_RDONLY)) < 1 || fstat(fd, &sbuf) < 0)
			exit(EXIT_FAILURE);
		if ((ptr = mmap(0, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
			exit(EXIT_FAILURE);
		len = strlen(ptr);
		fd_to_buf(STDOUT);
		ft_cat(fd);
		ret = read(fildes[0], &buf, len);
		if (ret != len || strncmp(buf, ptr, len))
			exit(EXIT_FAILURE);
		munmap(ptr, sbuf.st_size);
	);
	add_subtest(subtests, "Your ft_cat doesn't work with basic input", handle_status(0));
}

void	ft_cat_invalid_fd(t_subtest **subtests)
{
	SANDBOX(
		ft_cat(42000);
	);
	add_subtest(subtests, "Your ft_cat doesn't work with invalid file descriptor", handle_status(0));
}

void	cat_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_cat");
	subtests = get_last_subtest(tests);
	ft_cat_basic(subtests);
	ft_cat_invalid_fd(subtests);
}

///////////////////////////////
//         ft_strcpy         //
///////////////////////////////

void	ft_strcpy_basic(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(20);
		char	*s2 = malloc(20);

		s1 = memset(s1, 0x55, 20);
		s2 = memset(s2, 0x55, 20);
		s1 = strcpy(s1, "apocalypse 984");
		s2 = ft_strcpy(s2, "apocalypse 984");
		if (memcmp(s1, s2, 20))
			exit(EXIT_FAILURE);
		free(s1);
		free(s2);
	);
	add_subtest(subtests, "Your ft_strcpy doesn't work with basic input", handle_status(0));
}

void	ft_strcpy_first_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strcpy(NULL, "bye");
	);
	add_subtest(subtests, "Your ft_strcpy doesn't segfault with a null destination", handle_status(SIGSEGV));
}

void	ft_strcpy_second_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strcpy(malloc(1), NULL);
	);
	add_subtest(subtests, "Your ft_strcpy doesn't segfault with a null source", handle_status(SIGSEGV));
}

void	strcpy_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_strcpy");
	subtests = get_last_subtest(tests);
	ft_strcpy_basic(subtests);
	ft_strcpy_first_null(subtests);
	ft_strcpy_second_null(subtests);
}

///////////////////////////////
//        ft_strncpy         //
///////////////////////////////

void	ft_strncpy_basic(t_subtest **subtests)
{
	SANDBOX(
		char	*s1 = malloc(20);
		char	*s2 = malloc(20);

		s1 = memset(s1, 0x55, 20);
		s2 = memset(s2, 0x55, 20);
		s1 = strncpy(s1, "apocalypse 984", 18);
		s2 = ft_strncpy(s2, "apocalypse 984", 18);
		if (memcmp(s1, s2, 20))
			exit(EXIT_FAILURE);
		free(s1);
		free(s2);
	);
	add_subtest(subtests, "Your ft_strncpy doesn't work with basic input", handle_status(0));
}

void	ft_strncpy_first_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strncpy(NULL, "bye", 1);
	);
	add_subtest(subtests, "Your ft_strncpy doesn't segfault with a null destination", handle_status(SIGSEGV));
}

void	ft_strncpy_second_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strncpy(malloc(1), NULL, 1);
	);
	add_subtest(subtests, "Your ft_strncpy doesn't segfault with a null source", handle_status(SIGSEGV));
}

void	ft_strncpy_zero(t_subtest **subtests)
{
	SANDBOX(
		ft_strncpy(NULL, NULL, 0);
	);
	add_subtest(subtests, "Your ft_strncpy doesn't work with zero as len parameter", handle_status(0));
}

void	ft_strncpy_overflow(t_subtest **subtests)
{
	SANDBOX(
		ft_strncpy(malloc(1), NULL, 1);
	);
	add_subtest(subtests, "Your ft_strncpy doesn't segfault when the second parameter is too big", handle_status(SIGSEGV));
}

void	strncpy_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_strncpy");
	subtests = get_last_subtest(tests);
	ft_strncpy_basic(subtests);
	ft_strncpy_first_null(subtests);
	ft_strncpy_second_null(subtests);
	ft_strncpy_zero(subtests);
	ft_strncpy_overflow(subtests);
}

///////////////////////////////
//         ft_strchr         //
///////////////////////////////

void	ft_strchr_basic_found(t_subtest **subtests)
{
	SANDBOX(
		char	*s = ft_strchr("prendre des petits bouts de trucs", 'b');

		if (strcmp(s, "bouts de trucs"))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strchr doesn't work with basic input", handle_status(0));
}

void	ft_strchr_basic_not_found(t_subtest **subtests)
{
	SANDBOX(
		if (ft_strchr("prendre des petits bouts de trucs", 'z'))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strchr doesn't work with basic input", handle_status(0));
}

void	ft_strchr_multiple_found(t_subtest **subtests)
{
	SANDBOX(
		char	*s = ft_strchr("prendre des petits bouts de trucs et puis les assembler ensemble", 'b');

		if (strcmp(s, "bouts de trucs et puis les assembler ensemble"))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strchr doesn't work with multiple occurence of c parameter", handle_status(0));
}

void	ft_strchr_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strchr(NULL, 0x55);
	);
	add_subtest(subtests, "Your ft_strchr doesn't segfault with null parameter", handle_status(SIGSEGV));
}

void	strchr_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_strchr");
	subtests = get_last_subtest(tests);
	ft_strchr_basic_found(subtests);
	ft_strchr_basic_not_found(subtests);
	ft_strchr_multiple_found(subtests);
	ft_strchr_null(subtests);
}

///////////////////////////////
//         ft_strcmp         //
///////////////////////////////

void	ft_strcmp_equal_basic(t_subtest **subtests)
{
	SANDBOX(
		if (ft_strcmp(S_1, S_1) != strcmp(S_1, S_1))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strcmp doesn't work with basic input", handle_status(0));
}

void	ft_strcmp_different_basic(t_subtest **subtests)
{
	SANDBOX(
		if (ft_strcmp(S_1, "nope dude") != strcmp(S_1, "nope dude"))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strcmp doesn't work with basic input", handle_status(0));
}

void	ft_strcmp_equal_detect_end(t_subtest **subtests)
{
	SANDBOX(
		if (ft_strcmp(S_2, "bye") != strcmp(S_2, "bye"))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strcmp doesn't work with characters after the '\\0'", handle_status(0));
}

void	ft_strcmp_different_detect_end(t_subtest **subtests)
{
	SANDBOX(
		if (ft_strcmp(S_2, "nope dude") != strcmp(S_2, "nope dude"))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strcmp doesn't work with characters after the '\\0'", handle_status(0));
}

void	ft_strcmp_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strcmp(NULL, NULL);
	);
	add_subtest(subtests, "Your ft_strcmp doesn't segfault with null parameter", handle_status(SIGSEGV));
}

void	strcmp_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_strcmp");
	subtests = get_last_subtest(tests);
	ft_strcmp_equal_basic(subtests);
	ft_strcmp_different_basic(subtests);
	ft_strcmp_equal_detect_end(subtests);
	ft_strcmp_different_detect_end(subtests);
	ft_strcmp_null(subtests);
}

///////////////////////////////
//        ft_strrchr         //
///////////////////////////////

void	ft_strrchr_basic_found(t_subtest **subtests)
{
	SANDBOX(
		char	*s = ft_strrchr("prendre des petits bouts de trucs", 'b');

		if (strcmp(s, "bouts de trucs"))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strrchr doesn't work with basic input", handle_status(0));
}

void	ft_strrchr_basic_not_found(t_subtest **subtests)
{
	SANDBOX(
		if (ft_strrchr("prendre des petits bouts de trucs", 'z'))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strrchr doesn't work with basic input", handle_status(0));
}

void	ft_strrchr_multiple_found(t_subtest **subtests)
{
	SANDBOX(
		char	*s = ft_strrchr("prendre des petits bouts de trucs et puis les assembler ensemble", 'b');

		if (strcmp(s, "ble"))
			exit(EXIT_FAILURE);
	);
	add_subtest(subtests, "Your ft_strrchr doesn't work with multiple occurence of c parameter", handle_status(0));
}

void	ft_strrchr_null(t_subtest **subtests)
{
	SANDBOX(
		ft_strrchr(NULL, 0x55);
	);
	add_subtest(subtests, "Your ft_strrchr doesn't segfault with null parameter", handle_status(SIGSEGV));
}

void	strrchr_tests(t_test **tests)
{
	t_subtest	**subtests;

	add_test(tests, "ft_strrchr");
	subtests = get_last_subtest(tests);
	ft_strrchr_basic_found(subtests);
	ft_strrchr_basic_not_found(subtests);
	ft_strrchr_multiple_found(subtests);
	ft_strrchr_null(subtests);
}

///////////////////////////////
//           main            //
///////////////////////////////

int	main(int ac, char **av)
{
	t_test		*tests;

	binary_name = av[0];
	tests = NULL;

	// PART - 1

	bzero_tests(&tests);
	strcat_tests(&tests);
	isalpha_tests(&tests);
	isdigit_tests(&tests);
	isalnum_tests(&tests);
	isascii_tests(&tests);
	isprint_tests(&tests);
	toupper_tests(&tests);
	tolower_tests(&tests);
	puts_tests(&tests);

	// PART - 2

	strlen_tests(&tests);
	memset_tests(&tests);
	memcpy_tests(&tests);
	strdup_tests(&tests);

	// PART - 3

	cat_tests(&tests);

	// BONUS

	strcpy_tests(&tests);
	strncpy_tests(&tests);
	strchr_tests(&tests);
	strcmp_tests(&tests);
	strrchr_tests(&tests);

	display_results(tests);
}
