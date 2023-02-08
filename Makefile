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
	@sudo cp $(BIN) /bin/

uninstall: fclean
	@sudo rm /bin/$(BIN)	

.PHONY: all clean fclean re install uninstall
