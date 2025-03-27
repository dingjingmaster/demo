cflag   = -w -fPIE -fPIC `pkg-config --cflags Qt5Core Qt5Network`

curDir 	= $(shell pwd)

src     = storage.moc.cpp storage.cpp storage.h 
obj     = $(patsubst %.cpp, %.o, $(src))
target  = storage.run

rcc = /usr/lib64/qt5/bin/rcc
moc = /usr/lib64/qt5/bin/moc

all:$(target) 
	@rm -f *.o *.moc.cpp

%.moc.run:
	@echo $@

%.run:main.o $(obj)
	@g++ -std=c++11 `pkg-config --cflags --libs Qt5Core Qt5Network` -o $@ $^ $(cflag) 
	@echo "CC -o $@ $<"

%.o:%.cpp
	@g++ -o $@ -c $< $(cflag)

%.moc.cpp:%.h
	@$(moc) $(curDir)/$< > $(curDir)/$@

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -f *.moc
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt
	@rm -f `find -name "*.moc.cpp"`

