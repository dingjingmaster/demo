pwd=$(shell pwd)

ifeq ($(shell uname -o), Darwin)
	subdirs = object-c 
else ifeq ($(shell uname -o), GNU/Linux)
	subdirs = \
		version \
		learn
endif


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
