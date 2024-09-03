OBJ 	= mainwindow.o moc_mainwindow.o pieceslist.o  moc_pieceslist.o puzzlewidget.o moc_puzzlewidget.o 
SRC 	= mainwindow.cpp  mainwindow.h  pieceslist.cpp  pieceslist.h  puzzle.qrc  puzzlewidget.cpp  puzzlewidget.h
SRC1 	= moc_mainwindow.cpp  moc_pieceslist.cpp  moc_puzzlewidget.cpp 

CFLAGS = $(shell pkg-config --cflags Qt5Widgets Qt5Core Qt5Gui)
CFLAGS = `pkg-config --cflags Qt5Widgets Qt5Core Qt5Gui`
LIBS   = $(shell pkg-config --libs Qt5Widgets Qt5Core Qt5Gui)
LIBS   = `pkg-config --libs Qt5Widgets Qt5Core Qt5Gui`

rcc = /usr/lib64/qt5/bin/rcc
moc = /usr/lib64/qt5/bin/moc

all:puzzle.run


puzzle.run:main.o puzzle.o $(OBJ)
	@g++ -o $@ $(LIBS) $^
	@echo "CC -o $@ $^"

%.o:%.cpp
	@g++ -o $@ -c $(CFLAGS) $(LIBS) $^
	@echo "CC -o $@ $<"

moc_%.cpp : %.h
	@$(moc) $< > $@

puzzle.cpp : puzzle.qrc
	@$(rcc) $< -o $@ 


clean:
	@rm -f *.o
	@rm -f *.run
	@rm -f moc_*.cpp
