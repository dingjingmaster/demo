libc_path=$(shell find /usr/lib -name "libc.so.6")
my_libc_path=$(shell pwd)

all : libs test kylin

kylin : kylin_libs kylin_test

libs:
	gcc -fpic -shared -Wall -o hook-tools.so hook-tools.c `pkg-config --cflags --libs glib-2.0` -ldl -DLIBC=\"$(libc_path)\"

test:
	@echo -e "\n\nLD_PRELOAD=$(my_libc_path)/hook-tools.so && export LD_PRELOAD\n\n"

kylin_libs:
	g++ -fpic -shared -Wall -o hook-for-kylin.so hook-for-kylin.cpp `pkg-config --cflags --libs Qt5Core` -ldl

kylin_test:
	@echo -e "\n\nLD_PRELOAD=$(my_libc_path)/hook-for-kylin.so && export LD_PRELOAD\n\n"




.PHONY: clean libs all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
