cflag = -w

src = $(wildcard ncurses-*.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	cc -o $@ $^ $(cflag) dump_window.c `pkg-config --cflags --libs ncurses glib-2.0`

.PHONY:clean all

clean:
	@rm -fr *.run
