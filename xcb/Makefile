cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	cc -o $@ $^ $(cflag) -lxcb `pkg-config --cflags --libs xcb_atom`

.PHONY:clean all

clean:
	@rm -fr *.run
