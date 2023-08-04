cflag = -w 

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.cpp)))

target = $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target)

%.run:%.cpp
	g++ -o $@ $< 

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

