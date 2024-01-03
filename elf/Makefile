cflag = -w

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/elf-*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o
	gcc -o $@ $< `pkg-config --libs glib-2.0 libelf` $(cflag) 

%.o:%.c
	gcc `pkg-config --cflags glib-2.0 libelf` -o $@ -c $< $(cflag)

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

