NAME			=	libft.a
UNIT_TEST		=	lib_test

CFLAGS			=	-g
CC				=	gcc

SRCS_LIST		=	ft_puts.s ft_bzero.s ft_strcat.s ft_isalpha.s ft_isdigit.s \
					ft_isalnum.s ft_isascii.s ft_isprint.s ft_tolower.s ft_toupper.s \
					ft_strlen.s ft_memset.s ft_memcpy.s ft_strdup.s ft_cat.s ft_memccpy.s
SRCS_DIR		=	srcs/
SRCS			=	$(addprefix $(SRCS_DIR), $(SRCS_LIST))

UNIT_SRCS_LIST	=	main.c
UNIT_SRCS_DIR	=	unit_srcs/
UNIT_SRCS		=	$(addprefix $(UNIT_SRCS_DIR), $(UNIT_SRCS_LIST))

OBJS_LIST		=	$(patsubst %.s, %.o, $(SRCS_LIST))
OBJS_DIR		=	objs/
OBJS			=	$(addprefix $(OBJS_DIR), $(OBJS_LIST))

UNIT_OBJS_LIST	=	$(patsubst %.c, %.o, $(UNIT_SRCS_LIST))
UNIT_OBJS_DIR	=	unit_objs/
UNIT_OBJS		=	$(addprefix $(UNIT_OBJS_DIR), $(UNIT_OBJS_LIST))

.PHONY: test all re clean fclean

all: $(NAME)

$(NAME): $(OBJS)
	@rm -rf $(UNIT_TEST)
	@echo "\033[37mLinking...\033[0m"
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@echo "\033[32mLibrary \033[1;32m$(NAME)\033[1;0m\033[32m created.\033[0m"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.s
	@mkdir -p $(OBJS_DIR)
	@nasm -g -f macho64 $< -o $@
	@echo "\033[34mCompilation of \033[36m$(notdir $<)\033[34m done.\033[0m"

test: $(NAME) $(UNIT_TEST)

$(UNIT_TEST): $(UNIT_OBJS)
	@echo "\033[37mLinking...\033[0m"
	@$(CC) $(CFLAGS) -L . -lft $(UNIT_OBJS) -o $(UNIT_TEST)
	@echo "\033[32mBinary \033[1;32m$(UNIT_TEST)\033[1;0m\033[32m created.\033[0m"

$(UNIT_OBJS_DIR)%.o: $(UNIT_SRCS_DIR)%.c
	@mkdir -p $(UNIT_OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "\033[34mCompilation of \033[36m$(notdir $<)\033[34m done.\033[0m"

clean :
	@rm -rf $(OBJS_DIR)
	@rm -rf $(UNIT_OBJS_DIR)
	@echo "\033[31mObjects files \033[1;31m$(OBJS_LIST)\033[1;0m\033[31m removed.\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT_TEST)
	@echo "\033[31mLib \033[1;31m$(NAME)\033[1;0m\033[31m removed.\033[0m"

re: fclean all
