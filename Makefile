pwd=$(shell pwd)
subdirs= \
		 c core cppbase cbase cairo \
		 dbus dconf device \
		 function \
		 gdk3 gtk gudev gvfs gio glib gsettings gstreamer gio-channel gobject gtk \
		 ipc \
		 lightdm localization libblocked \
		 module \
		 pam polkit \
		 sl \
		 str \
		 vala vte \
		 x xcb \
		 syscall

all:
	@for subdir in $(subdirs);\
		do\
		cd $(pwd)/$$subdir && make;\
		done

clean:
	@for subdir in $(subdirs);\
		do\
		cd $(pwd)/$$subdir && make clean;\
		done


.PHONY:clean 

