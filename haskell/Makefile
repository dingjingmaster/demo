.PHONY:all clean

curDir  = $(shell pwd)

src     = $(wildcard $(curDir)/*.hs)
#src     = $(filter-out *libempty.c, $(src0))
target  = $(patsubst %.hs, %.run, $(src))

all:$(target)

%.run:%.hs
	@ghc -o $@ $^


clean:
	@rm -f *.o
	@rm -f *.hi
	@rm -f *.run
