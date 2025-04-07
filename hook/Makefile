libc_path=$(shell find /usr/lib -name "libc.so.6")
my_libc_path=$(shell pwd)

src1 = $(wildcard hook-*.c)
src2 = $(wildcard *.cpp)
target1 = $(patsubst %.c, %.so,   $(src1))
target2 = $(patsubst %.cpp, %.so, $(src2))

target  = wayland-hook.so
target += $(target1)
target += $(target2)

all:$(target)


wayland-hook.so:wayland-hook.c hook-common.c
	@gcc -fpic -shared -Wall -o $@ $^ -lwayland-client
	@echo -e "\n\n LD_PRELOAD=$(my_libc_path)/$@ && export LD_PRELOAD\n\n"

hook-qstyle.so : hook-qstyle.cpp
	g++ -fpic -shared -Wall -o $@ $< `pkg-config --cflags --libs Qt5Widgets` -ldl -DHOOK_COMMON=\"/usr/lib\"
	@echo -e "\n\nLD_PRELOAD=$(my_libc_path)/$@ && export LD_PRELOAD\n\n"

hook-x.so : hook-x.c libcommon-x.so
	gcc -fpic -shared -Wall -o $@ $< -ldl -DHOOK_COMMON=\"/usr/lib/\"
	@echo -e "\n\nLD_PRELOAD='$(my_libc_path)/$@' && export LD_PRELOAD\n\n"

hook-dnd.so : hook-dnd.c libcommon-dnd.so
	gcc -fpic -shared -Wall -o $@ $< -ldl -DHOOK_COMMON=\"/usr/lib\"
	@echo -e "\n\nLD_PRELOAD='$(my_libc_path)/$@' && export LD_PRELOAD\n\n"

%.so : %.c libcommon-hook.so
	gcc -fpic -shared -Wall -o $@ $< -ldl `pkg-config --cflags --libs glib-2.0` -DLIBC=\"$(libc_path)\" -DHOOK_COMMON=\"/usr/lib\"
	@echo -e "\n\nLD_PRELOAD=$(my_libc_path)/$@ && export LD_PRELOAD\n\n"

libcommon-x.so : common-x.c c-log.h
	gcc -fpic -shared -Wall -o $@ $^ `pkg-config --cflags --libs glib-2.0 x11`

libcommon-dnd.so : common-dnd.c c-log.h
	gcc -fpic -shared -Wall -o $@ $^ `pkg-config --cflags --libs glib-2.0 x11`

libcommon-hook.so : common-hook.c 
	gcc -fpic -shared -Wall -o $@ $^ `pkg-config --cflags --libs glib-2.0 x11 gtk+-3.0`


.PHONY: target src clean all

clean:
	@rm -f `find -name "*.o"`
	@rm -f `find -name "*.so"`
