cflag = -w -Wl,--no-as-needed

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/gl-*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)

%.run:%.o glad.o
	@gcc -o $@ $^ -pie -lpthread -lrt `pkg-config --libs glib-2.0 gl glfw3` $(cflag) 
	@echo "cc -o $@"

%.o:%.c
	@gcc -c $< `pkg-config --cflags glib-2.0 gl glfw3` $(cflag)
	@echo "cc -c $<"

.PRECIOUS:%.o
.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

