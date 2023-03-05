cflag = -w `pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets`

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.cpp)))

target = $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target)

%.run:%.cpp
	g++ -std=c++20 `pkg-config --libs Qt5Core Qt5Gui Qt5Widgets` -o $@ $< $(cflag) 

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

