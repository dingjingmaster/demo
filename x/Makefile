cflag = -w -lXmu -lXi

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	@cc -o $@ $^ $(cflag) `pkg-config --cflags --libs x11 gtk+-2.0 cairo xcb xcb-xkb xcb-xinput xinerama`
	@echo "cc -o $@"

.PHONY:clean all

clean:
	@rm -fr *.run
	@rm -fr *.png
	@rm -fr *.svg
