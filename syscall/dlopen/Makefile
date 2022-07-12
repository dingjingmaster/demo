libc_path=$(shell find /usr/lib -name "libc.so.6")
my_libc_path=$(shell pwd)

all:libs test


libs:
	gcc -fpic -shared -Wall -o dlopen-shared-lib.so dlopen-shared-lib.c -ldl -DLIBC=\"$(libc_path)\"

test:
	LD_PRELOAD=$(my_libc_path)/dlopen-shared-lib.so && export LD_PRELOAD && uptime


.PHONY: clean libs all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
