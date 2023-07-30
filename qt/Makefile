cflag = -w -fPIE -fPIC `pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets`

curDir = $(shell pwd)
src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.cpp)))
target = $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target)

ss.run : ss.cpp server-header.moc

%.run:%.cpp 
	g++ -std=c++11 `pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets Qt5Network` -o $@ $< $(cflag) 

%.o:%.cpp
	g++ -o $@ -c $< $(cflag)

%.moc:%.h
	moc $(curDir)/$< > $(curDir)/$@

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -f *.moc
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

