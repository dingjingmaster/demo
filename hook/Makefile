libc_path=$(shell find /usr/lib -name "libc.so.6")
my_libc_path=$(shell pwd)

src1 = $(wildcard hook-*.c)
src2 = $(wildcard *.cpp)
target1 = $(patsubst %.c, %.so,   $(src1))
target2 = $(patsubst %.cpp, %.so, $(src2))

all : $(target1) $(target2)

hook-qstyle.so : hook-qstyle.cpp
	g++ -fpic -shared -Wall -o $@ $< `pkg-config --cflags --libs Qt5Widgets` -ldl
	@echo -e "\n\nLD_PRELOAD=$(my_libc_path)/$@ && export LD_PRELOAD\n\n"


%.so : %.c
	gcc -fpic -shared -Wall -o $@ $< `pkg-config --cflags --libs glib-2.0 gdk-2.0` -ldl -DLIBC=\"$(libc_path)\"
	@echo -e "\n\nLD_PRELOAD=$(my_libc_path)/$@ && export LD_PRELOAD\n\n"



.PHONY: target src clean all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
