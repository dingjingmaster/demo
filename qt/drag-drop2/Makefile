OBJ 	= draglabel.o dragwidget.o 

CFLAGS = $(shell pkg-config --cflags Qt5Widgets Qt5Core Qt5Gui)
CFLAGS = `pkg-config --cflags Qt5Widgets Qt5Core Qt5Gui`
LIBS   = $(shell pkg-config --libs Qt5Widgets Qt5Core Qt5Gui)
LIBS   = `pkg-config --libs Qt5Widgets Qt5Core Qt5Gui`

all:fridgemagnets.run


fridgemagnets.run:main.o fridgemagnets.o $(OBJ)
	@g++ -o $@ $(LIBS) $^
	@echo "CC -o $@ $^"

%.o:%.cpp
	@g++ -o $@ -c $(CFLAGS) $(LIBS) $^
	@echo "CC -o $@ $<"

moc_%.cpp : %.h
	@moc-qt5 $< > $@

fridgemagnets.cpp : fridgemagnets.qrc
	@rcc-qt5 $< -o $@ 


clean:
	@rm -f *.o
	@rm -f *.run
	@rm -f moc_*.cpp
