cflag = -g -w #-std=gnu11

curDir 	= $(shell pwd)

src 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/demo-*.c)))
target 	= $(strip $(patsubst %.c, %.run, $(src)))


all:$(target)

%.run:%.o ./aes.o ./aes_lib.o
	@gcc $(cflag) -o $@ $^ `pkg-config --libs glib-2.0 gio-2.0 openssl`
	@echo "cc -o $@"

%.o:%.c
	@gcc $(cflag) `pkg-config --cflags glib-2.0 gio-2.0 openssl` -o $@ -c $< $(cflag)
	@echo "cc -o $@"


.PHONY:clean all empty-func.run libempty.run

clean:
	@find -name "*.a" -delete
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete
	@find -name "*.log" -delete
	@find -name "*.out" -delete
	@find -name "*.data" -delete

