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

install: all
	@sudo cp $(BIN) /usr/local/bin

uninstall: fclean
	@sudo rm /usr/local/bin/$(BIN)	

.PHONY: all clean fclean re install uninstall
