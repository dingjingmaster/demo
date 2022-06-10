cflag = -w \
		-I/usr/include/glib-2.0

demo1_src = read_write_file.c

gobject_target = \
	udisk-format.run		\
	read_write_file.run 	\

target = $(gobject_target)

all:$(target)

udisk-format.run:udisk-format.o
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs udisks2 gtk+-2.0`

read_write_file.run:read_write_file.o
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs glib-2.0 gio-2.0 gobject-2.0`

%.o:%.c
	cc -o $@ -c $< $(cflag) `pkg-config --cflags --libs glib-2.0 gio-2.0 gobject-2.0 udisks2 gtk+-2.0`

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

