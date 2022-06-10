cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

%.run:%.c
	gcc -o $@ $^ $(cflag) -l crypto -l ssl

.PHONY:clean all

clean:
	@rm -fr *.run
