CC=		gcc
CFLAGS= -g3 -Wall -Werror -Wextra
BIN=	pomo
SRC=	src/main.c\
		src/flag.c

OBJ=	$(SRC:.c=.o)