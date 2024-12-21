src1 = $(wildcard hook-*.c)
target1 = $(patsubst %.c, %.so, $(src1))

all : hook-clipboard.so

hook-clipboard.so:hook-clipboard.c hook-common.c
	gcc -shared -fPIC -o $@ $^ -ldl -lX11

.PHONY: target src clean all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
