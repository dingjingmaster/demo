.PHONY:all

all:
	@echo 'FEATURES(make features): '
	@echo $(.FEATURES)
	@echo 'MAKECMDGOALS(user input): '
	@echo $(MAKECMDGOALS)
	@echo 'MAKEFILE_LIST'
	@echo $(MAKEFILE_LIST)
	@echo 'lastword: ' $(lastword $(MAKEFILE_LIST))
	@echo 'realpath: ' $(realpath $(dir $(MAKEFILE_LIST)))
	@echo 'CURDIR  : ' $(CURDIR)
