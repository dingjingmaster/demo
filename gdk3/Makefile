cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	cc -o $@ $^ $(cflag) -lX11 `pkg-config --cflags --libs gtk+-2.0 glib-2.0 gdk-2.0`

.PHONY:clean all

clean:
	@rm -fr *.run
	@rm -fr *.png
