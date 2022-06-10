curDir = $(shell pwd)

cflag = \
	-w 					\
	-I$(curDir)/../../ 	\

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)
	echo $(target)


%.run:%.o
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs glib-2.0 gio-2.0 gobject-2.0`

%.o:%.c
	cc -o $@ -c $< $(cflag) `pkg-config --cflags --libs glib-2.0 gio-2.0 gobject-2.0`

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

