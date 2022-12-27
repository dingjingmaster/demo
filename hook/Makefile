libc_path=$(shell find /usr/lib -name "libc.so.6")
my_libc_path=$(shell pwd)

all : libs test


libs:
	gcc -fpic -shared -Wall -o hook-tools.so hook-tools.c `pkg-config --cflags --libs glib-2.0` -ldl -DLIBC=\"$(libc_path)\"

test:
	@echo -e "\n\nLD_PRELOAD=$(my_libc_path)/hook-tools.so && export LD_PRELOAD\n\n"


.PHONY: clean libs all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
