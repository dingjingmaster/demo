cflag = -w #-std=gnu11

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/ev-*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o
	@gcc -o $@ $< -lpthread -lrt -lev $(cflag) 
	@echo "CC -o $@ -lev $<"

%.o:%.c
	@gcc `pkg-config --cflags glib-2.0 gio-2.0 openssl` -o $@ -c $< $(cflag)
	@echo "CC -o $@ $<"

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

