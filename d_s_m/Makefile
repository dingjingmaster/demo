CUR_DIR = $(realpath $(shell pwd))

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.cpp)))
src2 = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.c)))

target = $(strip $(patsubst %.cpp, %.run, $(src)))
target2 = $(strip $(patsubst %.c, %.run, $(src2)))

cflags = -L $(CUR_DIR) -Wl,-rpath=$(CUR_DIR) `pkg-config --cflags --libs gio-2.0` 

D_S_M:$(target2)

all:$(target) ${target2}

%.run:%.cpp
	g++ -std=c++11 -D CURDIR=\""$(CUR_DIR)"\" -o $@ $< $(cflags) -lfileclock

%.run:%.c
	gcc -D CURDIR=\""$(CUR_DIR)"\" -o $@ $< $(cflags) -lmonsafe

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

