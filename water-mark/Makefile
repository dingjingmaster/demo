pwd=$(shell pwd)


x11SrcFile = src/main.c src/x11.c


all : water-mark.run
	

water-mark.run : ${x11SrcFile}
	gcc -o $@ $^ `pkg-config --cflags --libs x11 cairo xfixes xinerama xrandr xext`



.PHONY: all water-mark.run
