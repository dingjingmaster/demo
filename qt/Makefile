cflag = -w -fPIE -fPIC `pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets Qt5WebSockets Qt5Concurrent`

curDir 	= $(shell pwd)

src11	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.cpp)))
src12	= $(filter-out *_moc.cpp, $(src11))
src13	= $(filter-out *.moc.cpp, $(src12))
target1	= $(strip $(patsubst %.cpp, %.run, $(src13)))

src21 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/hook-*.c)))
src22	= $(filter-out *_moc.cpp, $(src21))
src23	= $(filter-out *.moc.cpp, $(src22))
target2 = $(strip $(patsubst %.c, %.so, $(src23)))

rcc = /usr/lib64/qt5/bin/rcc
moc = /usr/lib64/qt5/bin/moc

all:$(target1) $(target2)
	@echo $(target1) $(target2)
	#make -C drag-drop1
	#make -C drag-drop2

ss.run : ss.cpp server-header.moc.cpp
	g++ -std=c++11 `pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets Qt5Network` -o $@ $^ $(cflag) 
	@echo CC -o $@


header.run:header.cpp header/header-button.cpp header/main-header.cpp
	@$(rcc) -o header/res.h header/res.qrc
	@$(moc) -o header/main-header.moc.cpp header/main-header.h
	@$(moc) -o header/header-button.moc.cpp header/header-button.h
	@g++ -std=c++11 `pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets Qt5Network` -o $@ $^ header/main-header.moc.cpp header/header-button.moc.cpp $(cflag) 
	@echo CC -o $@

%.so : %.c
	@g++ -fpic -shared -Wall -o $@ $< `pkg-config --cflags --libs Qt5Gui Qt5Widgets gio-2.0` -ldl
	@echo "CC -o $@ $<"

%.moc.run:
	@echo $@

%.run:%.cpp 
	g++ -std=c++11 `pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets Qt5Network gio-2.0` -o $@ $< $(cflag) 
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
	make -C drag-drop1 clean
	make -C drag-drop2 clean

