cflag = -w -fPIE -fPIC `pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets`

curDir 	= $(shell pwd)
src 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.cpp)))
target 	= $(strip $(patsubst %.cpp, %.run, $(src)))

src1 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/hook-*.c)))
target1 = $(strip $(patsubst %.c, %.so, $(src1)))

all:$(target) $(target1)

ss.run : ss.cpp server-header.moc

header.run:header.cpp header/header-button.cpp header/main-header.cpp
	@rcc -o header/res.h header/res.qrc
	@moc -o header/main-header.moc.cpp header/main-header.h
	@moc -o header/header-button.moc.cpp header/header-button.h
	@g++ -std=c++11 `pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets Qt5Network` -o $@ $^ header/main-header.moc.cpp header/header-button.moc.cpp $(cflag) 

%.so : %.c
	@g++ -fpic -shared -Wall -o $@ $< `pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets` -ldl
	@echo "CC -o $@ $<"

%.run:%.cpp 
	@g++ -std=c++11 `pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets Qt5Network` -o $@ $< $(cflag) 
	@echo "CC -o $@ $<"

%.o:%.cpp
	@g++ -o $@ -c $< $(cflag)

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

