cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs x11 gtk+-2.0 cairo xcb`

.PHONY:clean all

clean:
	@rm -fr *.run
	@rm -fr *.png
	@rm -fr *.svg
