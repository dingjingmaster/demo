cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	gcc -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-2.0 gio-unix-2.0`

.PHONY:clean all

clean:
	@rm -fr *.run
