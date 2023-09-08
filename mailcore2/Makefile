cflag = -w 

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.cpp)))

target = $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target)

%.run:%.cpp
	g++ -o $@ $< -lm -lpthread -lrt -lsqlite3 -lMailCore `pkg-config --cflags --libs glib-2.0 gio-2.0 icu-i18n icu-io icu-uc tidy libxml-2.0 uuid libetpan libctemplate openssl` $(cflag) 

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

