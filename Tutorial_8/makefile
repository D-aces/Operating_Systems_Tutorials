# Makefile for all T8 programs

CC = gcc
CFLAGS = -Wall -Wextra

all: q1.o q2.o

q1.o: Q1.c
	$(CC) $(CFLAGS) -o q1.o Q1.c

q2.o: Q2.c
	$(CC) $(CFLAGS) -o q2.o Q2.c

.PHONY: clean
clean:
	rm -f q1.o q2.o

