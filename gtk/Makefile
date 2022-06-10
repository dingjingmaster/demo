cflag = -w -g

gtk2_src = $(wildcard gtk2/gtk2_*.c)
gtk2_target = $(patsubst %.c, %.run, $(gtk2_src))

gtk3_src = $(wildcard gtk3/gtk3_*.c)
gtk3_target = $(patsubst %.c, %.run, $(gtk3_src))

gtk4_src = $(wildcard gtk4/gtk4_*.c)
gtk4_target = $(patsubst %.c, %.run, $(gtk4_src))

lightdm_src = $(wildcard lightdm/lightdm_*.c)
lightdm_target = $(patsubst %.c, %.run, $(lightdm_src))

all:$(gtk3_target) $(gtk2_target) $(gtk4_target)

gtk2:$(gtk2_target)

gtk3:$(gtk3_target)

gtk4:$(gtk4_target)

lightdm:$(lightdm_target)

gtk2_%.run:gtk2_%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-2.0`

gtk3_%.run:gtk3_%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs gtk+-3.0 gtk4`

gtk4_%.run:gtk4_%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags gtk4 --libs gtk4`

lightdm_%.run:lightdm_%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags gtk+-3.0 liblightdm-gobject-1 --libs gtk+-3.0 liblightdm-gobject-1`

.PHONY:clean all

clean:
	@rm -f */*.run
