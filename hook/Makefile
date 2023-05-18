libc_path=$(shell find /usr/lib -name "libc.so.6")
my_libc_path=$(shell pwd)

src = $(wildcard *.c)
target = $(patsubst %.c, %.so, $(src))

all : $(target)

%.so : %.c
	gcc -fpic -shared -Wall -o $@ $< `pkg-config --cflags --libs glib-2.0 gdk-2.0` -ldl -DLIBC=\"$(libc_path)\"
	@echo -e "\n\nLD_PRELOAD=$(my_libc_path)/$@ && export LD_PRELOAD\n\n"


.PHONY: target src clean all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
