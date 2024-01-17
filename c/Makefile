cflag = -w #-std=gnu11

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o
	@gcc -o $@ $< -lpthread -lrt -lsqlite3 `pkg-config --cflags --libs glib-2.0 gio-2.0 openssl` $(cflag) 
	@echo "cc -o $@"

%.o:%.c
	@gcc `pkg-config --cflags glib-2.0 gio-2.0 openssl` -o $@ -c $< $(cflag)
	@echo "cc -o $@"

.PHONY:clean all

clean:
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete

