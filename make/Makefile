curDir = $(shell pwd)
mk = $(wildcard $(curDir)/*.mk)

all:$(mk)
	@$(foreach f, $(mk), make -f $(f);)


.PHONY:$(mk)
