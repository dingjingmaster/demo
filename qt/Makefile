cflag = -w -fPIE -fPIC `pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets`

CUR_DIR = $(shell pwd)

src = $(strip $(subst $(CUR_DIR), ., $(wildcard $(CUR_DIR)/*.cpp)))

target = $(strip $(patsubst %.cpp, %.run, $(src)))

all:$(target)

%.run:%.cpp mocs
	g++ -std=c++11 `pkg-config --cflags --libs Qt5Core Qt5Gui Qt5Widgets Qt5Network` -o $@ $< $(cflag) 

%.o:%.cpp
	g++ -o $@ -c $< $(cflag)

mocs:
	moc $(CUR_DIR)/server-header.h > $(CUR_DIR)/server-header.moc

.PHONY:clean all

clean:
	@rm -fr *.o
	@rm -fr */*.o
	@rm -fr *.run
	@rm -fr */*.run
	@rm -fr *.txt
	@rm -fr */*.txt

