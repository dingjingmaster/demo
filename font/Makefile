cflag = -w -Wl,--no-as-needed

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/font-*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o 
	@gcc -o $@ $^ -pie `pkg-config --libs x11 glib-2.0 pango pangocairo` $(cflag) 
	@echo "cc -o $@"

%.o:%.c
	@gcc -c $< `pkg-config --cflags glib-2.0 pango pangocairo` $(cflag)
	@echo "cc -c $<"

.PRECIOUS:%.o
.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr *.txt
	@rm -fr */*.run
	@rm -fr */*.txt

