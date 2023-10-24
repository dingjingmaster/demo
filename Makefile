pwd=$(shell pwd)

ifeq ($(shell uname -o), Darwin)
	subdirs = object-c 
else ifeq ($(shell uname -o), GNU/Linux)
	subdirs = \
		c core cppbase cbase cairo cuda \
		dbus dconf device \
		file function \
		gdk3 gtk gudev gvfs gio glib gsettings gstreamer gio-channel gobject gtk gjs gnome-shell \
		ipc iso \
		lightdm localization libblocked \
		module \
		pam pcre polkit protobuf python-c-api\
		qt-demo qt-tools \
		sl sqlite\
		str \
		vala vte \
		x xcb \
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
