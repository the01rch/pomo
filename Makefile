include config.mk

all:	$(BIN)

$(BIN):	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ)

fclean: clean
	rm $(BIN)

re: fclean all

.PHONY: all clean fclean re
