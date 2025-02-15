curDir 	= $(shell pwd)
src    	= $(wildcard $(curDir)/*.cpp)
targets = $(patsubst %.cpp, %.run, $(src))
ldflags = 
flags 	= -g -w -I/usr/local/include/gq/ -I/usr/local/include/
libs 	= -Wl,-rpath=/usr/local/lib -L /usr/local/lib -lgumbo -lgq

all: $(targets) 
	@echo $(targets)
	

%.run:%.cpp
	g++ $(flags) -o $@ $^ $(ldflags) $(libs)

clean:
	rm -rf $(targets)

.PHONY:clean all
