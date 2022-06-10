cflag = -w

src = $(wildcard *.c)
target_run = $(patsubst %.c, %.run, $(src))
target_po = $(patsubst %.c, %.po, $(src))
target_mo = $(patsubst %.c, %.mo, $(src))

all:po $(target_run)

%.run:%.c
	cc -o $@ $^ $(cflag) `pkg-config --cflags --libs glib-2.0`

po:$(target_po)

%.po:%.pot
	mv $< $@

install:$(target_mo)
	mkdir -p po/zh_CN.UTF-8/LC_MESSAGES/
	mv *.mo po/zh_CN.UTF-8/LC_MESSAGES/

%.pot:%.c
	xgettext --add-comments --keyword=_ $< -o $@ --from-code=UTF-8

%.mo:%.po
	msgfmt $< -o $@

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr *.po
	@rm -fr po/
	@rm -fr *.mo
	@rm -fr *.pot
	@rm -fr *.run
