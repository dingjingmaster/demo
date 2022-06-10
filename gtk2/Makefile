CC = gcc
cflag = `pkg-config --cflags --libs gtk+-2.0`

src = $(wildcard ./*.c)

target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c 
	$(CC) -o $@ $^ $(cflag)

clean:
	rm -f *.run
