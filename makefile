NAME = minishell

CC = gcc

CFLAGS = -Wall -Werror -Wextra

# Includes
INCLUDES = -I ./includes/

# Directories
SRC_DIR = ./src/
OBJS_DIR = ./objs/
LIBFT_DIR = ./libft/

# Source Files
SRC_FILES = lexer.c \
			parser.c \
			main.c \
			aux.c \
			executor.c \
			terminal.c \
			builtin.c \
			env.c \
			signals.c

# Object Files
OBJS_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJS_DIR),$(OBJS_FILES))

# Libraries
LIBFT = $(LIBFT_DIR)libft.a
LIBRARIES = -lft -lreadline

all: ${NAME}

$(LIBFT):
	make bonus -C $(LIBFT_DIR)

# creating executables
${NAME}: $(LIBFT) $(OBJS_DIR) $(OBJS) 
	${CC} ${CFLAGS} ${OBJS} -L${LIBFT_DIR} ${LIBRARIES} -o ${NAME}

# Creating Object file folder
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Creating object files
$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	${CC} ${CFLAGS} -c ${INCLUDES} $< -o $@

clean:
	rm -rf $(OBJS_DIR)
	rm -rf ./libft/*.o

fclean: clean
	rm -rf ${NAME} out* valgrind*
	rm -rf ./libft/*.a

re: fclean ${NAME}

rebonus: fclean bonus

.PHONY: all clean fclean re