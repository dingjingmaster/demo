cflag = -w -lXmu

src = $(wildcard *.c)
target = drag-drop.run #$(patsubst %.c, %.run, $(src))

all:$(target)

%.run:$(src)
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs x11 gtk+-2.0 cairo xcb`

.PHONY:clean all

clean:
	@rm -fr *.run
	@rm -fr *.png
	@rm -fr *.svg
