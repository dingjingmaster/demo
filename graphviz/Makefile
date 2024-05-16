curDir = $(shell pwd)

src = $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.dot)))

target = $(strip $(patsubst %.dot, %.svg, $(src)))

all:$(target)

%.svg:%.dot
	@dot -T svg -o $@ $<
	@echo "$@"

.PHONY:clean all

clean:
	@find -name "*.svg" -delete
	@find -name "*.png" -delete

