all:sub_make_done
	@echo 'Main build done'

sub_make_done:
	@echo 'execute'
	touch $@
