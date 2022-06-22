cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	gcc -o $@ $^ $(cflag) -lX11 `pkg-config --cflags --libs vte-2.91`


.PHONY:clean all

clean:
	@rm -fr *.run
