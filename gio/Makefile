cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	@gcc $^ $(cflag) `pkg-config --cflags --libs gtk+-3.0 gdk-3.0 gio-2.0 glib-2.0 gio-unix-2.0 udisks2` -o $@
	@echo "CC -O $@ $^"

.PHONY:clean all

clean:
	@rm -fr *.run
