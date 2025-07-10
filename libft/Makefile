NAME = libft.a
AR = ar -rcs
CC = gcc

# Directories
SRC_DIR = #./src/
OBJS_DIR = #./objs/
BIN_DIR	= #./bin/

SRC_FILES = ft_strjoin.c \
		ft_strlen.c \
		ft_substr.c \
		ft_strtrim.c \
		ft_split.c \
		ft_strdup.c \
		ft_itoa.c \
		ft_strmapi.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memccpy.c \
		ft_memmove.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strnstr.c \
		ft_strncmp.c \
		ft_atoi.c \
		ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_calloc.c \
		ft_striteri.c \
		get_next_line.c \

SRC_BONUS_FILES = ft_lstnew_bonus.c \
		ft_lstadd_front_bonus.c \
		ft_lstsize_bonus.c \
		ft_lstlast_bonus.c \
		ft_lstadd_back_bonus.c \
		ft_lstdelone_bonus.c \
		ft_lstclear_bonus.c \
		ft_lstiter_bonus.c \
		ft_lstmap_bonus.c \

#TESTFILE = test.c

# CC Flags
CFLAGS = -Wall -Wextra -Werror

# Add directory prefix to source files
#SRC = $(addprefix $(SRC_DIR),$(SRC_FILES))
#SRC_BONUS = $(addprefix $(SRC_DIR),$(BONUS_FILES))

# Include directory flags
INCLUDES = #-I ./includes/

# Object files
OBJS_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJS_DIR), $(OBJS_FILES))
OBJS_BONUS_FILES = $(SRC_BONUS_FILES:.c=.o)
OBJS_BONUS = $(addprefix $(OBJS_DIR), $(OBJS_BONUS_FILES))
#OBJS_TEST_FILE = $(addprefix $(OBJS_DIR), $(TESTFILE:.c=.o))

# Binaries
#BIN_TEST = $(addprefix $(BIN_DIR), test)

all: ${NAME}

${NAME}: ${OBJS}
	${AR} ${NAME} ${OBJS}

bonus: ${OBJS} ${OBJS_BONUS}
	${AR} ${NAME} ${OBJS} ${OBJS_BONUS}

#test: ${OBJS_DIR} ${BIN_DIR} ${OBJS} ${OBJS_BONUS} ${OBJS_TEST_FILE}
#	${CC} ${CFLAGS} ${OBJS} ${OBJS_BONUS} ${OBJS_TEST_FILE} -o ${BIN_TEST} -lbsd

#$(OBJS_DIR):
#	mkdir -p $(OBJS_DIR)

#$(BIN_DIR):
#	mkdir -p $(BIN_DIR)

%.o: %.c
	${CC} ${CFLAGS} -c ${INCLUDES} $< -o $@

clean:
	rm -f $(addprefix ${SRC_DIR},${OBJS_FILES}) ${OBJS} ${OBJS_TEST} ${OBJS_BONUS}

fclean: clean
	rm -rf ${BIN_DIR} ${OBJS_DIR}
	rm -f ${NAME} test

re: fclean all

#retest: fclean test

.PHONY: all clean fclean re
