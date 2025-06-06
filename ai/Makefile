bin = plane-game.run panda_assistant.run

rcc = /usr/lib64/qt5/bin/rcc
moc = /usr/lib64/qt5/bin/moc

all:$(bin)

plane-game.run:plane-game.c
	gcc -o $@ $^ `pkg-config --cflags --libs ncurses`

panda_assistant.run:panda_assistant.cpp panda_assistant.moc
	g++ -o $@ $< -fPIC `pkg-config --cflags --libs Qt5Core Qt5Widgets Qt5Gui opengl`

%.moc:%.cpp
	$(moc) -o $@ $<

clean:
	@rm -f *.run *.moc
