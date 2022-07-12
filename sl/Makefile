CC=gcc
CFLAGS=-O -Wall

all: sl

sl: sl.c sl.h
	$(CC) $(CFLAGS) -o sl.run sl.c -lncurses

clean:
	@rm -f sl
	@rm -f *.run

distclean: clean
