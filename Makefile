#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsankola <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/20 20:58:24 by tsankola          #+#    #+#              #
#    Updated: 2023/05/20 20:58:26 by tsankola         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = pipex

NAME_BONUS = pipex

CC = cc

CFLAGS = -Wall -Wextra -Werror

SFLAGS = -fsanitize=address -g3

SRCDIR = src

LIBDIR = lib

OBJDIR = obj

INCDIR = include

TEST1 = test/1.i "/bin/cat" "/usr/bin/wc -l" 1.out
TEST2 = test/2.i sort "wc -m" 2.out
TEST3 = test/1.i "test/3.sleeptest.sh" "wc -m" 3.out
TEST4 = test/4.hardlink.i cat "wc -l" 4.out
TEST5 = test/5.symlink.i cat "wc -l" 5.out
TEST6 = test/6.brokelink.i cat "wc -l" 6.out

LIB_INCDIR = $(LIBDIR)/$(INCDIR)

_SRC = pipex.c pipex_helpers.c pipes.c pipex_searchers.c pipex_proc_hdr.c \
		top_helpers.c middle_helpers.c
SRC = $(patsubst %, $(SRCDIR)/%, $(_SRC))

_OBJ = $(patsubst %.c, %.o, $(_SRC))
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

_SRC_BONUS = pipex_bonus.c pipex_helpers_bonus.c pipes_bonus.c \
		pipex_searchers_bonus.c pipex_heredoc_bonus.c
SRC_BONUS = $(patsubst %, $(SRCDIR)/%, $(_SRC_BONUS))

_OBJ_BONUS = $(patsubst %.c, %.o, $(_SRC_BONUS))
OBJ_BONUS = $(patsubst %, $(OBJDIR)/%, $(_OBJ_BONUS))

_LIB = libft.a
LIB = $(patsubst %, $(LIBDIR)/%, $(_LIB))
LIB_NAME = $(patsubst lib%.a, %, $(_LIB))

_INC = pipex.h pipex_proc_hdr.h
INC = $(patsubst %, $(INCDIR)/%, $(_INC))

_INC_BONUS = pipex_bonus.h pipex_heredoc_bonus.h
INC_BONUS = $(patsubst %, $(INCDIR)/%, $(_INC_BONUS))

_LIB_INC = libft.h
LIB_INC = $(patsubst %, $(LIB_INCDIR)/%, $(_LIB_INC))

.PHONY: all bonus test clean

all: $(NAME)

bonus: $(OBJ_BONUS) $(INC_BONUS) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_BONUS) -L$(LIBDIR) -l$(LIB_NAME) -o $(NAME_BONUS)

sanitizer: $(OBJ)
	$(CC) $(CFLAGS) $(SFLAGS) $(OBJ) -L$(LIBDIR) -l$(LIB_NAME) -o $(NAME)

suppress: $(SRC) $(LIB) $(INC)
	$(CC) $(SRC) -I$(INCDIR) -I$(LIB_INCDIR) -L$(LIBDIR) -l$(LIB_NAME) -o $(NAME)

test: $(NAME)
	$(addprefix ./, $(NAME)) $(TEST1)
	$(addprefix ./, $(NAME)) $(TEST2)
	$(addprefix ./, $(NAME)) $(TEST3)
	$(addprefix ./, $(NAME)) $(TEST4)
	$(addprefix ./, $(NAME)) $(TEST5)
	$(addprefix ./, $(NAME)) $(TEST6)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIB):
	$(MAKE) -C $(LIBDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INC) $(LIB_INC) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -I$(INCDIR) -I$(LIB_INCDIR) -o $@

$(NAME): $(OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -l$(LIB_NAME) -o $@

clean:
	rm -f $(OBJ)
	rm -f $(OBJ_BONUS)
	rmdir $(OBJDIR) 2> /dev/null || true
	$(MAKE) -C $(LIBDIR) clean
	rm -f $(LIB)
	rm -f $(patsubst %, %~, $(SRC) $(SRC_BONUS))
	rm -f $(patsubst %, %~, $(INC) $(INC_BONUS))

fclean: clean
	rm -f $(NAME)

re: fclean all