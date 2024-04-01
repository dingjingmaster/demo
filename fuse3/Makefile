cflag = -g -w #-std=gnu11

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o
	@gcc -o $@ $< `pkg-config --cflags --libs glib-2.0 fuse3` $(cflag) 
	@echo "cc -o $@"

%.o:%.c
	@gcc `pkg-config --cflags glib-2.0 fuse3` -o $@ -c $< $(cflag)
	@echo "cc -o $@"

.PHONY:clean all

clean:
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete
	@find -name "*.log" -delete
	@find -name "*.data" -delete

