CUR_DIR = $(realpath $(shell pwd))

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.cpp)))

target = $(strip $(patsubst %.cpp, %.run, $(src)))

cflags = -L $(CUR_DIR) -laes -Wl,-rpath=$(CUR_DIR) `pkg-config --cflags --libs gio-2.0` 

all:$(target)

%.run:%.cpp
	g++ -std=c++11 -D CURDIR=\""$(CUR_DIR)"\" -o $@ $< $(cflags) 

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

