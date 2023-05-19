# TODO header

NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror

AR = ar

AFLAGS = rcs

INCDIR = include

SRCDIR = src

OBJDIR = obj

_SRC =	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
		ft_isascii.c ft_isdigit.c ft_isprint.c ft_isspace.c ft_itoa16.c \
		ft_itoa.c ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c \
		ft_lstdelone.c ft_lstiter.c ft_lstlast.c ft_lstmap.c \
		ft_lstnew.c ft_lstsize.c ft_memchr.c ft_memcmp.c \
		ft_memcpy.c ft_memmove.c ft_memset.c ft_printf.c ft_putchar_fd.c \
		ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c \
		ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
		ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c \
		ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
		ft_uitoa.c ft_ulitoa16.c
SRC =	$(patsubst %, $(SRCDIR)/%, $(_SRC))

_INC =	libft.h
INC =	$(patsubst %, $(INCDIR)/%, $(_INC))

_OBJ =	$(patsubst %.c, %.o, $(_SRC)) #SRC:%.c=%.o)
OBJ =	$(patsubst %, $(OBJDIR)/%, $(_OBJ))

.PHONY: all

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INC) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -I$(INCDIR) -o $@

$(NAME): $(OBJ)
	$(AR) $(AFLAGS) $@ $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all