curDir 	= $(CURDIR)
mk  	= $(wildcard $(curDir)/*.mk)
mkDir 	= cmdline make-call make-curdir make-if 

all:$(mk) $(mkDir)
	@$(foreach f, $(mk), make -f $(f);)
	@$(foreach f, $(mkDir), make -C $(f);)


.PHONY:$(mk)
