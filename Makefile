CC=gcc
CFLAGS= -g3 -W -Wall -O3
LDFLAGS= -lm
EXEC= Flow
SRC= $(wildcard src/*.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

Flow: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper 

run:
	@cd bin/
	@./Flow ../test 8

clean:
	@rm -rf obj/*.o

mrproper: clean
	@rm -rf $(EXEC)
