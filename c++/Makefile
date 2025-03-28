cflag = -w -g 

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.cpp)))

target = $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target)

demo-type.run:
	@echo "demo-type.run"

%.run:%.cpp
	@g++ -std=c++20 -o $@ $< $(cflag) 
	@echo "cc -o $@"

%.o:%.cpp
	@g++ -o $@ -c $< $(cflag)

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

