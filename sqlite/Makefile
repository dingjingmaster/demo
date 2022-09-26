cflag = -w 

CUR_DIR = $(shell pwd)

src 		= $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.c)))
src2 		= $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.cpp)))

target 		= $(strip $(patsubst %.c, %.run, $(src)))
target2 	= $(strip $(patsubst %.cpp, %.run, $(src2)))

all:$(target) ${target2}

prepare:
	sqlite3 -init $(CUR_DIR)/sqlite.sql

%.run:%.cpp
	g++ -o $@ $< `pkg-config --cflags --libs glib-2.0 gio-2.0` -lsqlite3 $(cflag)

%.run:%.o
	cc -o $@ $< -lrt -lsqlite3 `pkg-config --cflags --libs glib-2.0 gio-2.0 openssl` $(cflag) 

%.o:%.c
	cc `pkg-config --cflags --libs glib-2.0 gio-2.0` -o $@ -c $< $(cflag)


.PHONY:clean all

clean:
	@rm -f .lock
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt
	@rm -f */*.sqlite
	@rm -f *.sqlite

