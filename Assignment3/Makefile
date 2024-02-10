CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: cpu-scheduler

cpu-scheduler: cpu-scheduler.c
	@echo "compiling"
	$(CC) $(CFLAGS) -o cpu-scheduler cpu-scheduler.c
	@echo "executing"
	./cpu-scheduler process.txt

clean:
	@echo "removing the executables"
	rm -f cpu-scheduler
