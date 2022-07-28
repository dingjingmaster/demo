pwd=$(shell pwd)

subdirs = combox

all:
	@for subdir in $(subdirs);\
		do\
		cd $(pwd)/$$subdir && make -f $(pwd)/$$subdir/Makefile.dj;\
		done

clean:
	@for subdir in $(subdirs);\
		do\
		cd $(pwd)/$$subdir && make -f $(pwd)/$$subdir/Makefile.dj clean;\
		done


.PHONY:clean 

