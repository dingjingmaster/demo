cflag = -g -w #-std=gnu11

curDir = $(shell pwd)

src0 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
src1 	= $(filter-out *libempty.c, $(src0))
src  	= $(filter-out icmp-ping2.c, $(src1))
target 	= $(strip $(patsubst %.c, %.run, $(src)))


all:$(target)

icmp-ping2.run:

icmp-ping2.o:

empty-func.run:libempty.a empty-func.o
#	@gcc $(cflag) -o $@ empty-func.o -L. -lempty
#	@echo "cc -o $@"

libempty.run:

enca.run:enca.c
	@gcc $(cflag) -o $@ $< -lpthread -lrt -lsqlite3 `pkg-config --libs glib-2.0 gio-2.0 gmodule-2.0 enca`
	@echo "cc -o $@"

read-file.run:read-file.c
	@gcc $(cflag) -o $@ $< -lpthread -lrt -lsqlite3 `pkg-config --libs glib-2.0 gio-2.0 gmodule-2.0 enca`
	@echo "cc -o $@"

%.run:%.o
	@gcc $(cflag) -o $@ $< -lpthread -lrt -lsqlite3 `pkg-config --libs glib-2.0 gio-2.0 gmodule-2.0 openssl libcap`
	@echo "cc -o $@"

%.a:%.o
	@ar rcs $@ $<

%.o:%.c
	@gcc $(cflag) `pkg-config --cflags glib-2.0 gio-2.0 openssl libpcap` -o $@ -c $< $(cflag)
	@echo "cc -o $@ -c $<"


.PHONY:clean all empty-func.run libempty.run

clean:
	@find -name "*.a" -delete
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete
	@find -name "*.log" -delete
	@find -name "*.data" -delete

