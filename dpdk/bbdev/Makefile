cflag = -w #-std=gnu11

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.c
	@gcc -o $@ $< `pkg-config --cflags --libs libdpdk` $(cflag) 
	@echo "cc -o $@"

.PHONY:clean all

clean:
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete

