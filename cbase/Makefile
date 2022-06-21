cflag = -w 

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo-*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o
	cc -o $@ $< $(cflag) 

%.o:%.c
	cc -o $@ -c $< $(cflag)

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

