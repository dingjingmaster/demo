cflag = -g -w #-std=gnu11

curDir = $(shell pwd)

src0 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
src  	= $(filter-out *libempty.c, $(src0))
target 	= $(strip $(patsubst %.c, %.run, $(src)))


all:$(target)


empty-func.run:libempty.a empty-func.o
#	@gcc $(cflag) -o $@ empty-func.o -L. -lempty
#	@echo "cc -o $@"

libempty.run:


%.run:%.o
	@gcc $(cflag) -o $@ $< -lpthread -lrt -lsqlite3 `pkg-config --libs glib-2.0 gio-2.0 gmodule-2.0 openssl`
	@echo "cc -o $@"

%.a:%.o
	@ar rcs $@ $<

%.o:%.c
	@gcc $(cflag) `pkg-config --cflags glib-2.0 gio-2.0 openssl libcap` -o $@ -c $< $(cflag)
	@echo "cc -o $@"


.PHONY:clean all empty-func.run libempty.run

clean:
	@find -name "*.a" -delete
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete
	@find -name "*.log" -delete
	@find -name "*.data" -delete

