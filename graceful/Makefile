defines = -DCOMPILE_LIBGRACEFUL -DQT_XML_LIB -DQT_DBUS_LIB -DQT_SVG_LIB -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_-DQT_NO_DEBUG -DQT_NO_KEYWORDS -DQT_WIDGETS_LIB -DQT_X11EXTRAS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_XM -DQT_DBUS_LIB -DQT_CORE_LIB

cflag = -w -fPIC -D_REENTRANT $(defines)

src = $(wildcard *.cpp)
target = $(patsubst %.cpp, %.run, $(src))

all:$(target)

%.run:%.cpp
	g++ -o $@ $^ $(cflag) -lX11 -lprocps -lXss -lgraceful `pkg-config --cflags --libs gio-2.0 glib-2.0 Qt5Core Qt5DBus Qt5Gui Qt5Network Qt5Xdg Qt5Widgets Qt5X11Extras`

.PHONY:clean all

clean:
	@rm -fr *.run
