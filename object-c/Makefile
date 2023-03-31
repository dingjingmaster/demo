cflags = -w `gnustep-config --objc-flags` -lgnustep-base -lobjc

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.m)))

target = $(strip $(patsubst %.m, %.run, $(src)))

all:$(target)

%.run:%.m
	gcc $(cflags) -o $@ $< 


.PHONY:clean all

clean:
	@rm -fr *.d
	@rm -fr *.run
	@rm -fr *.txt

