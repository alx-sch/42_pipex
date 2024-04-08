# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aschenk <aschenk@student.42berlin.de>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/05 19:39:15 by aschenk           #+#    #+#              #
#    Updated: 2024/04/05 17:04:41 by aschenk          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		pipex

SRCS :=		src/main.c \
			src/pipeline.c \
			src/call_cmd.c \
			src/utils.c
OBJS :=		$(SRCS:src/%.c=obj/%.o)
HDRS := 	incl/pipex.h

LIBFT_DIR =	src/libft
LIBFT =		obj/libft/libft.a

CC =		cc
CFLAGS =	-Wall -Wextra -Werror -Iincl
LIBS = 		-Lobj/libft -lft

TOTAL_SRCS := $(words $(SRCS))
SRC_NUM := 0

# Define ANSI escape codes for colors and styles
RESET = \033[0m
BOLD = \033[1m
RED = \033[31;2m
GREEN = \033[32m
YELLOW = \033[33m

# Target 'all' is the default target, building program specified by $(NAME).
all:	$(NAME)

# Target $(NAME) depends on object files $(OBJS) and libft library.
$(NAME):	$(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(BOLD)$(YELLOW)\n$(NAME) successfully compiled.$(RESET)"

# Rule to define how to generate object files (%.o) from corresponding
# source files (%.c). Each .o file depends on the associated .c file and the
# project header file (incl/project.h)
# -c:		Generates o. files without linking.
# -o $@:	Output file name;  '$@' is replaced with target name (the o. file).
# -$<:		Represents the first prerequisite (the c. file).
obj/%.o: src/%.c $(HDRS)
	@mkdir -p $(@D)
	@$(eval SRC_NUM := $(shell expr $(SRC_NUM) + 1))
	@$(eval PERCENT := $(shell printf "%.0f" $(shell echo "scale=4; $(SRC_NUM) / $(TOTAL_SRCS) * 100" | bc)))
	@printf "$(BOLD)\rCompiling $(NAME): ["
	@$(eval PROGRESS := $(shell expr $(PERCENT) / 5))
	@printf "$(GREEN)%0.s#$(RESET)$(BOLD)" $(shell seq 1 $(PROGRESS))
	@if [ $(PERCENT) -lt 100 ]; then printf "%0.s-" $(shell seq 1 $(shell expr 20 - $(PROGRESS))); fi
	@printf "] "
	@if [ $(PERCENT) -eq 100 ]; then printf "$(GREEN)"; fi
	@printf "%d%% $(RESET)" $(PERCENT)
	@$(CC) $(CFLAGS) -c -o $@ $<

# Build the libft library by calling make in the src/libft directory
# (-C changes directory). This target will be executed if libft.a is missing or
# if any of the .c files in the src/libft directory are modified.
$(LIBFT):	$(LIBFT_DIR)/ft_isalpha.c \
			$(LIBFT_DIR)/ft_isdigit.c \
			$(LIBFT_DIR)/ft_isalnum.c \
			$(LIBFT_DIR)/ft_isascii.c \
			$(LIBFT_DIR)/ft_isprint.c \
			$(LIBFT_DIR)/ft_strlen.c \
			$(LIBFT_DIR)/ft_toupper.c \
			$(LIBFT_DIR)/ft_tolower.c \
			$(LIBFT_DIR)/ft_strncmp.c \
			$(LIBFT_DIR)/ft_strlcpy.c \
			$(LIBFT_DIR)/ft_strlcat.c \
			$(LIBFT_DIR)/ft_strchr.c \
			$(LIBFT_DIR)/ft_strrchr.c \
			$(LIBFT_DIR)/ft_strnstr.c \
			$(LIBFT_DIR)/ft_memset.c \
			$(LIBFT_DIR)/ft_memchr.c \
			$(LIBFT_DIR)/ft_memcpy.c \
			$(LIBFT_DIR)/ft_memcmp.c \
			$(LIBFT_DIR)/ft_memmove.c \
			$(LIBFT_DIR)/ft_bzero.c \
			$(LIBFT_DIR)/ft_atoi.c \
			$(LIBFT_DIR)/ft_strdup.c \
			$(LIBFT_DIR)/ft_calloc.c \
			$(LIBFT_DIR)/ft_substr.c \
			$(LIBFT_DIR)/ft_strjoin.c \
			$(LIBFT_DIR)/ft_strtrim.c \
			$(LIBFT_DIR)/ft_split.c \
			$(LIBFT_DIR)/ft_itoa.c \
			$(LIBFT_DIR)/ft_strmapi.c \
			$(LIBFT_DIR)/ft_striteri.c \
			$(LIBFT_DIR)/ft_putchar_fd.c \
			$(LIBFT_DIR)/ft_putstr_fd.c \
			$(LIBFT_DIR)/ft_putendl_fd.c \
			$(LIBFT_DIR)/ft_putnbr_fd.c \
			$(LIBFT_DIR)/ft_lstnew.c \
			$(LIBFT_DIR)/ft_lstadd_front.c \
			$(LIBFT_DIR)/ft_lstsize.c \
			$(LIBFT_DIR)/ft_lstlast.c \
			$(LIBFT_DIR)/ft_lstadd_back.c \
			$(LIBFT_DIR)/ft_lstdelone.c \
			$(LIBFT_DIR)/ft_lstclear.c \
			$(LIBFT_DIR)/ft_lstiter.c \
			$(LIBFT_DIR)/ft_lstmap.c \
			$(LIBFT_DIR)/ft_strcmp.c \
			$(LIBFT_DIR)/ft_isbinary.c \
			$(LIBFT_DIR)/get_next_line_bonus.c \
			$(LIBFT_DIR)/ft_printf_utils.c \
			$(LIBFT_DIR)/ft_printf.c \
			$(LIBFT_DIR)/libft.h
	@mkdir -p obj/libft
	@echo ""
	@make -s -C $(LIBFT_DIR)

# Target to remove all generated files.
clean:
	@rm -rf obj
	@echo "$(BOLD)$(RED)Object files and libft.a removed.$(RESET)"

# Target to remove all generated files and the program executable.
fclean:	clean
	@rm -f $(NAME)
	@echo "$(BOLD)$(RED)$(NAME) removed.$(RESET)"

# Target to remove all generated files, the program executable,
# and then rebuild the program.
re:	fclean all

.PHONY: all clean fclean re
