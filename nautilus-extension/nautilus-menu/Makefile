curdir = $(shell pwd)
src = $(strip $(subst $(curdir), ., $(wildcard $(curdir)/*.c)))
obj = $(strip $(patsubst %.c, %.o, $(src)))
cflags = -shared -fPIC

all:libiandasec-menu.so

libiandasec-menu.so:$(obj)
	@gcc -o $@ 	$^ $(cflags) `pkg-config --libs libnautilus-extension`
	@echo "CC -o $@ $^"

%.o:%.c
	@gcc -o $@ -c 	$< $(cflags) `pkg-config --cflags libnautilus-extension`
	@echo "CC -o $@ $<"

install:libiandasec-menu.so
	@sudo cp ./libiandasec-menu.so /usr/lib/nautilus/extensions-3.0/
	@echo 'install OK!'

clean:
	@rm -rf *.o
	@rm -rf *.so

.PHONY:clean all install
