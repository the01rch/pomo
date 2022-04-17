BIN =	pomo

SRC =	src/main.go

all:
	go build -o $(BIN) $(SRC)
	mv $(BIN) bin

run:
	@bin/./pomo $(filter-out $@,$(MAKECMDGOALS))
%:
	@true

clean:
	@go clean
	@rm bin/$(BIN)

re: clean all
