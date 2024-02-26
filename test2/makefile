CC = gcc
CFLAGS = -Wall -std=c11

all: parent

parent: Parent.c
	@echo "compiling"
	$(CC) $(CFLAGS) -o child Child.c
	$(CC) $(CFLAGS) -o parent Parent.c
	@echo "executing"
	./parent 2 10000000 1

clean:
	@echo "removing the executables"
	@rm -f parent
	@rm -f child
