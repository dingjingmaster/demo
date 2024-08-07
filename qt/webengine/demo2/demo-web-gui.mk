.PHONY:moc all
src = demo-web-gui.o moc_web-gui.o web-gui.o ../lib/Configure.o ../qtwebgui/WebExportCore.o ../qtwebgui/moc_WebExportCore.o
LIBS = `pkg-config --libs Qt5Widgets Qt5X11Extras Qt5WebEngineWidgets` -lX11
CXXFLAGS = `pkg-config --cflags Qt5Widgets Qt5X11Extras Qt5WebEngineWidgets` -I. -I../include

all:$(src)
	g++ -o demo-web-gui.run $^ $(LIBS)

web-gui.o:web-gui.cpp
	g++ -o $@ $(CXXFLAGS) -c $^

%.o:%.c
	g++ -o $@ $(CXXFLAGS) -c $^

moc_%.c:%.h
	moc -o $@ $<


