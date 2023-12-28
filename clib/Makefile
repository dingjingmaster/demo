cflag = -w 

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/demo*.c)))

target = $(strip $(patsubst %.c, %.run, $(src)))

all:$(target)


demo1-main.run : demo1-main.o lib1-constructor.so
	@gcc -o $@ $< -Wl,-rpath=$(CUR_DIR) -L $(CUR_DIR) -l1-constructor $(cflag)
	@echo "CC -o $@"

lib1-constructor.so:lib1-constructor.c
	@gcc -o $@ $< $(cflag) --shared -fpic
	@echo "CC -o $@"

%.run:%.o
	@gcc -o $@ $< -lpthread -lrt -lsqlite3 `pkg-config --cflags --libs glib-2.0 gio-2.0 openssl` $(cflag) 
	@echo "CC -o $@"

%.o:%.c
	@gcc `pkg-config --cflags glib-2.0 gio-2.0 openssl` -o $@ -c $< $(cflag)
	@echo "CC -o $@"

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -rf *.so
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

