pwd=$(shell pwd)

ifeq ($(shell uname -o), Darwin)
	subdirs = object-c 
else ifeq ($(shell uname -o), GNU/Linux)
	subdirs = \
		c core clib cppbase cbase cairo cuda cups \
		dbus dconf device dpdk \
		ev elf \
		file font function fuse3 \
		gdk3 gtk gudev gvfs gio glib gsettings gstreamer gio-channel gobject gtk gjs gnome-shell \
		hook hook-module \
		ipc iso \
		lightdm localization libblocked \
		module \
		pam pcre polkit protobuf python-c-api python \
		qt qt-demo qt-tools \
		rust \
		sl sqlite\
		str \
		vala vte \
		x xcb x-drag-drop \
		syscall
endif


all:
	@for subdir in $(subdirs); \
		do \
		cd $(pwd)/$$subdir && make; \
		done

clean:
	@for subdir in $(subdirs);\
		do\
		cd $(pwd)/$$subdir && make clean;\
		done


.PHONY:clean all build clean
