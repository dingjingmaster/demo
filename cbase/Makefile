cflag = -w 

CUR_DIR = $(shell pwd)

src_p = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo2-*-p.c)))
src2 = $(filter-out $(src_p), $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo2-*.c))))
src = $(filter-out $(src2) $(src_p), $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo-*.c))))

target2 = $(strip $(patsubst %.c, %.run, $(src2)))
target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target) $(target2)

demo2-%.run:demo2-%.c $(CUR_DIR)/demo2-%-p.c
	cc -o $@ $^ $(cflag) 

demo-%.run:demo-%.c
	cc -o $@ $< $(cflag) 

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

