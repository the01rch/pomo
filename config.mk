CC=		gcc
CFLAGS= -g3 -Wall -Werror -Wextra
BIN=	pomo
SRC=	src/pomo.c\
		src/util.c

OBJ=	$(SRC:.c=.o)
