cflag = -w 

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.c
	gcc -o $@ $< $(cflag) `pkg-config --cflags --libs gtk+-2.0 gio-2.0 glib-2.0 gio-unix-2.0 udisks2` -ludev

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

