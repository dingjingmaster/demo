cflag = -w

src = $(wildcard *.c)
target = $(patsubst %.c, %.run, $(src))

all:$(target)

server-connect.run:server-connect.c
	cc -o $@ $^ `pkg-config --cflags --libs wayland-server`

client-connect.run:client-connect.c
	cc -o $@ $^ `pkg-config --cflags --libs wayland-client`

%.run:%.c
	@wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml $@-xdg-shell-protocol.c
	@wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell-client-protocol.h
	cc -o $@ $^ $@-xdg-shell-protocol.c $(cflag) `pkg-config --cflags --libs wayland-client` -lrt
	rm -f $@-xdg-shell-protocol.c xdg-shell-client-protocol.h

.PHONY:clean all

clean:
	@rm -rf *.run
	@rm -rf *.png
	@rm -rf *.svg
	@rm -rf *-xdg-shell-protocol.*
	@rm -rf xdg-shell-client-protocol.h
