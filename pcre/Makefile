cflag = -w 

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.cpp)))

target = $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target)

%.run:%.o
	g++ -o $@ `pkg-config --cflags --libs libpcre2-8` $(cflag) $^ 

%.o:%.cpp
	g++ -o $@ -c $< $(cflag)

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

