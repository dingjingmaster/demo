cflag = -g -w 

curDir = $(shell pwd)

src 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
#src  	= $(filter-out icmp-ping2.c, $(src1))
target 	= $(patsubst %.c, %.run, $(src))

all:$(target)


%.run:%.c
	@gcc $(cflag) -o $@ $^ -larchive
	@echo CC -o $@

clean:
	@rm -f *.run *.tar.gz

.PHONY:all clean 
