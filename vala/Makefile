cflag = 

src = $(wildcard *.vala)
target = $(patsubst %.vala, %.run, $(src))

all:$(target)

%.run:%.vala
	valac --pkg gio-2.0 -o $@ $^ $(cflag)

.PHONY:clean all

clean:
	@rm -fr *.run
