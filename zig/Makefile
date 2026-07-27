curDir = $(shell pwd)

src 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.zig)))
target 	= $(strip $(patsubst %.zig, %.run, $(src)))


all:$(target)

%.run:%.zig
	zig build-exe $< -femit-bin=$@

clean:
	@rm -f *.run
