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

LIB_INCDIR = $(LIBDIR)/$(INCDIR)

_SRC = pipex.c pipex_helpers.c pipex_searchers.c pipex_proc_hdr.c \
		top_helpers.c middle_helpers.c bottom_helpers.c parsing.c
SRC = $(patsubst %, $(SRCDIR)/%, $(_SRC))

_OBJ = $(patsubst %.c, %.o, $(_SRC))
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

_SRC_BONUS = pipex_bonus.c pipex_helpers_bonus.c pipex_proc_hdr_bonus.c \
		pipex_searchers_bonus.c pipex_heredoc_bonus.c top_helpers_bonus.c \
		middle_helpers_bonus.c bottom_helpers_bonus.c \
		pipex_character_buffer_bonus.c parsing_bonus.c
SRC_BONUS = $(patsubst %, $(SRCDIR)/%, $(_SRC_BONUS))

_OBJ_BONUS = $(patsubst %.c, %.o, $(_SRC_BONUS))
OBJ_BONUS = $(patsubst %, $(OBJDIR)/%, $(_OBJ_BONUS))

_LIB = libft.a
LIB = $(patsubst %, $(LIBDIR)/%, $(_LIB))
LIB_NAME = $(patsubst lib%.a, %, $(_LIB))

_INC = pipex.h pipex_proc_hdr.h
INC = $(patsubst %, $(INCDIR)/%, $(_INC))

_INC_BONUS = pipex_bonus.h pipex_proc_hdr_bonus.h pipex_heredoc_bonus.h
INC_BONUS = $(patsubst %, $(INCDIR)/%, $(_INC_BONUS))

_LIB_INC = libft.h
LIB_INC = $(patsubst %, $(LIB_INCDIR)/%, $(_LIB_INC))

.PHONY: all bonus test clean

all: $(NAME)

bonus: .bonus

.bonus: $(OBJ_BONUS) $(INC_BONUS) $(LIB)
	$(CC) $(CFLAGS) $(OBJ_BONUS) -L$(LIBDIR) -l$(LIB_NAME) -o $(NAME_BONUS)
	@touch .bonus
	@rm -f .mandatory

sanitizer: $(OBJ)
	$(CC) $(CFLAGS) $(SFLAGS) $(OBJ) -L$(LIBDIR) -l$(LIB_NAME) -o $(NAME)

suppress: $(SRC) $(LIB) $(INC)
	$(CC) $(SRC) -I$(INCDIR) -I$(LIB_INCDIR) -L$(LIBDIR) -l$(LIB_NAME) \
	-o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIB):
	$(MAKE) -C $(LIBDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INC) $(LIB_INC) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -I$(INCDIR) -I$(LIB_INCDIR) -o $@

$(NAME): .mandatory

.mandatory: $(OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -l$(LIB_NAME) -o $(NAME)
	@touch .mandatory
	@rm -f .bonus

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
	@rm -f .bonus
	@rm -f .mandatory

re: fclean all
