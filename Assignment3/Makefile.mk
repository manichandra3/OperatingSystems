CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: cpu-scheduler

cpu-scheduler: cpu-scheduler.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f cpu-scheduler
