cflag = -w -g

gtk3_src = $(wildcard gtk3_*.c)
gtk3_target = $(patsubst %.c, %.run, $(gtk3_src))

all:$(gtk3_target)

gtk3:$(gtk3_target)

gtk3_%.run:gtk3_%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-3.0 gtk4`

.PHONY:clean all

clean:
	@rm -f *.run
