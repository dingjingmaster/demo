cflag = -g -w #-std=gnu11

curDir = $(shell pwd)

src 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
target 	= $(strip $(patsubst %.c, %.run, $(src)))


all:$(target)


%.run:%.o
	@gcc $(cflag) -o $@ $< `pkg-config --libs glib-2.0 gio-2.0 gmodule-2.0 opencc`
	@echo "cc -o $@"

%.o:%.c
	@gcc $(cflag) `pkg-config --cflags glib-2.0 gio-2.0 opencc` -o $@ -c $< $(cflag)
	@echo "cc -o $@ -c $<"


.PHONY:clean all 

clean:
	@find -name "*.a" -delete
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete
	@find -name "*.log" -delete
	@find -name "*.data" -delete

