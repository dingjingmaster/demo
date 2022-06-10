cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	gcc -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-3.0 gstreamer-1.0 gstreamer-video-1.0 gio-2.0 glib-2.0`

.PHONY:clean all

clean:
	@rm -fr *.run
