cflag = -w 

CUR_DIR = $(shell pwd)

special = ./demo-func1.c  ./demo-func1-p.c

src1 = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo-*.c)))
src = $(filter-out $(special), $(src1))


target = $(strip $(patsubst %.c, %.run, $(src)))

# 重定义
#target += demo-func1.run

#tt:
#	echo $(src)

all:$(target)

demo-func1.run:demo-func1.c demo-func1-p.c
	cc -o $@ $^ $(cflag) 

%.run:%.c
	cc -o $@ $< $(cflag) 

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

