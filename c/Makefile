cflag = -g -w #-std=gnu11

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o
	@gcc $(cflag) -o $@ $< -lpthread -lrt -lsqlite3 `pkg-config --cflags --libs glib-2.0 gio-2.0 openssl`
	@echo "cc -o $@"

%.o:%.c
	@gcc $(cflag) `pkg-config --cflags glib-2.0 gio-2.0 openssl` -o $@ -c $< $(cflag)
	@echo "cc -o $@"

.PHONY:clean all

clean:
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete
	@find -name "*.log" -delete
	@find -name "*.data" -delete

