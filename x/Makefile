cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	cc -o $@ $^ $(cflag) -lX11 `pkg-config --libs gtk+-2.0 --cflags cairo xcb`

.PHONY:clean all

clean:
	@rm -fr *.run
	@rm -fr *.png
	@rm -fr *.svg
