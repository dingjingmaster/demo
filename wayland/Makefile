cflag = -w

src = $(filter-out wayland-demo-protocol.*, $(wildcard *.c))
target = $(patsubst %.c, %.run, $(src))

all:$(target) wayland-demo.xml.run

wayland-demo-protocol.run:


wayland-demo.xml.run:wayland-demo.xml
	wayland-scanner private-code $< wayland-demo-protocol.c
	wayland-scanner client-header $< wayland-demo-protocol.h

server-connect.run:server-connect.c
	cc -o $@ $^ `pkg-config --cflags --libs wayland-server`

client-connect.run:client-connect.c
	cc -o $@ $^ `pkg-config --cflags --libs wayland-client`

%.run:%.c
	@wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml $@-xdg-shell-protocol.c
	@wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell-client-protocol.h
	cc -o $@ $^ $@-xdg-shell-protocol.c $(cflag) `pkg-config --cflags --libs wayland-client` -lrt
	rm -f $@-xdg-shell-protocol.c xdg-shell-client-protocol.h

.PHONY:clean all *.run

clean:
	@rm -rf *.run
	@rm -rf *.png
	@rm -rf *.svg
	@rm -f wayland-demo-protocol.*
	@rm -rf *-xdg-shell-protocol.*
	@rm -rf xdg-shell-client-protocol.h
