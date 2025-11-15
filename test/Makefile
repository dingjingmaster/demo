cflag = -g -w #-std=gnu11

curDir = $(shell pwd)

src0 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
src1 	= $(filter-out *libempty.c, $(src0))
src  	= $(filter-out icmp-ping2.c, $(src1))
target 	= $(strip $(patsubst %.c, %.run, $(src)))


all:$(target)


%.run:%.o
	@gcc $(cflag) -o $@ $< 
	@echo "cc -o $@"

%.a:%.o
	@ar rcs $@ $<

%.o:%.c
	@gcc $(cflag) -o $@ -c $< 
	@echo "cc -o $@ -c $<"


.PHONY:clean all 

clean:
	@find -name "*.a" -delete
	@find -name "*.o" -delete
	@find -name "*.run" -delete
	@find -name "*.txt" -delete
	@find -name "*.log" -delete
	@find -name "*.data" -delete

