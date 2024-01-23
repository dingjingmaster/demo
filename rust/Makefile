cflag = -w #-std=gnu11

curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.rs)))

target = $(strip $(patsubst %.rs, %.run, $(src)))

all:$(target)

%.run:%.rs
	@rustc $< -o $@
	@echo "rustc -o $@"

.PHONY:clean all

clean:
	@find -name "*.run" -delete
	@find -name "*.txt" -delete

