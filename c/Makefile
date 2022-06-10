cflag = -w 

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o
	cc -o $@ $< -lrt -lsqlite3 `pkg-config --cflags --libs glib-2.0 gio-2.0 openssl` $(cflag) 

%.o:%.c
	cc `pkg-config --cflags --libs glib-2.0 gio-2.0` -o $@ -c $< $(cflag)

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

