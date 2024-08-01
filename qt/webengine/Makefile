cflags = -std=c++11 -w -fPIE -fPIC `pkg-config --cflags Qt5Core Qt5Gui Qt5WebEngineWidgets Qt5Widgets`
libs   = `pkg-config --libs Qt5Core Qt5Gui Qt5WebEngineWidgets Qt5Widgets`

curDir 	= $(shell pwd)
src 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.cpp)))
target 	= $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target) 

%.run:%.cpp
	@g++ -o $@ $^ $(cflags) $(libs)
	@echo "CC -o $@ $<"

%.moc:%.h
	@moc $(curDir)/$< > $(curDir)/$@

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

