#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

int		ft_puts(const char *s);
void	ft_bzero(void *s, size_t n);
char	*ft_strcat(void *dest, void *src);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
char	ft_tolower(int c);
char	ft_toupper(int c);
char	*ft_strdup(const char *s1);
void	ft_cat(int fd);

typedef	struct			s_test
{
	char				*name;
	struct s_subtest	*subtests;
	struct s_test		*next;
}						t_test;

typedef	struct			s_subtest
{
	char				*explenation;
	int8_t				result;
	struct s_subtest	*next;
}						t_subtest;

int				status;
pid_t			pid;

int				_stdout;
int				fildes[2];

t_tests			*tests = NULL;
char			*g_res[7] = {
	"OK",
	"KO",
	"CRASH",
	"NO-SEGV",
	"BUS",
	"SEGV",
	"ABRT"
};

#define OK			0
#define KO			1
#define CRASH		2
#define NO_SEGV		3
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

#define SANDBOX(x)	if (!(pid = fork())) { x; exit(EXIT_SUCCESS); } else { wait(&status); }

t_subtest	*new_subtest(char *explenation, int8_t result)
{
	t_subtest	*new;

	if (!(new = malloc(sizeof(t_subtest))))
		return (NULL);
	new->next = NULL;
	new->explenation = strdup(explenation);
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
		return (NULL);
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_test(name);
}

void		add_subtest(t_subtest **subtests, char *explenation, int8_t result)
{
	t_subtest	*tmp;

	tmp = *subtests;
	if (!tmp)
	{
		*subtests = new_sutest(explanation, result);
		return (NULL);
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_subtest(explenation, result);
}

void	fd_to_buf(int fd)
{
	_stdout = dup(fd);
	if (pipe(fildes))
		return ;
	dup2(fildes[1], fd);
	close(fildes[1]);
}

void	handle_status(int expect)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == expect)
			return (OK);
		else if (expect)
			return (NO_SEGV);
		else if (sig == SIGSEGV)
			return (SEGV);
		else if (sig == SIGBUS)
			return (BUS);
		else if (sig == SIGABRT)
			return (ABRT);
		else
			return (CRASH);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		return (OK);
	else
		return (KO);
}

void	dispay_result(t_test *tests)
{
	while (tests)
	{
		printf("%s:", tests->name);
		while (tests->subtests)
		{
			printf(" %s[%s]%s", tests->subtests->result ? CKO : COK, g_res[tests->subtests->result], EOC);
			tests->subtests = tests->subtests->next;
		}
		tests = tests->next;
	}
}

void	(t_tests *tests)
{
	while (tests->next)
		tests = tests->next;
	return (tests->subtests);
}

///////////////////////////////
//          ft_puts          //
///////////////////////////////

void	ft_puts_basic()
{
	SANDBOX(
		int		len = strlen(S_1) + 1;
		char	buf[len];
		int		ret;

		fd_to_buf(STDOUT);
		ft_puts(S_1);
		ret = read(fildes[0], &buf, len + 1);
		if (ret != len || strncmp(buf, S_1, len - 1) || buf[ret - 1] != '\n')
			exit(EXIT_FAILURE);
	);
	handle_status(0);
}

void	ft_puts_detect_end()
{
	SANDBOX(
		int		len = strlen(S_2) + 1;
		char	buf[len];
		int		ret;

		fd_to_buf(STDOUT);
		ft_puts(S_2);
		ret = read(fildes[0], &buf, len + 1);
		if (ret != len || strncmp(buf, S_2, len - 1) || buf[ret - 1] != '\n')
			exit(EXIT_FAILURE);
	);
	handle_status(0);
}

void	puts_tests(t_test **tests)
{
	t_subtest	*subtests;

	add_test(tests, "ft_puts");
	subtests = get_last_subtest(*tests);
	ft_puts_basic(&subtest);
	ft_puts_detect_end(&subtest);
}

int	main()
{
	t_test		*tests;

	tests = NULL;
	puts_tests(&tests);
	display_result();
}
