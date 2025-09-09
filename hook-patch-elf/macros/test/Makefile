.PHONY:all clean
curDir = $(shell pwd)

cflags 	= -I. -I..
src 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
target 	= $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.c
	@gcc -o $@ $< $(cflags)
	@echo "CC -o $@"

clean:
	@rm -rf *.o
	@rm -rf *.run
	@echo 'clean done'
