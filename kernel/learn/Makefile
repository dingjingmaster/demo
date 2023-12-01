pwd 	:= $(shell pwd)

subdirs  = $(shell find . -maxdepth 1 -mindepth 1 -type d -not -path '(*/\.*)' -printf "%f\n" | xargs)


all:
	@for subdir in $(subdirs); \
		do \
		cd $(pwd)/$$subdir && make; \
		done

clean:
	@for subdir in $(subdirs);\
		do\
		cd $(pwd)/$$subdir && make clean;\
		done


.PHONY:clean all build clean
