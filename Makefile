NAME = get_next_line.a
MANDATORY = get_next_line.c get_next_line_utils.c

OBJ_PATH = ${MANDATORY:.c=.o}
BONUS_PATH = ${BONUS:.c=.o}

CC = cc 
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

.PHONY: all clean fclean re bonus

$(NAME): ${OBJ_PATH}
	ar rcs ${NAME} ${OBJ_PATH}
all: ${NAME}

clean: 
	${RM} ${OBJ_PATH} ${BONUS_PATH}

fclean: clean
	${RM} ${NAME}

re: fclean all

bonus: ${OBJ_PATH} ${BONUS_PATH}
	ar rcs ${NAME} ${OBJ_PATH} ${BONUS_PATH}