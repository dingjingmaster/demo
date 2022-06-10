cflag = -w -g

src = $(wildcard greeter-*.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-3.0 glib-2.0 liblightdm-gobject-1`

.PHONY:clean all

clean:
	@rm -f *.run
