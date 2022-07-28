pwd=$(shell pwd)

subdirs = demo1

all:
	@for subdir in $(subdirs);\
		do\
		cd $(pwd)/$$subdir && make;\
		done

clean:
	@for subdir in $(subdirs);\
		do\
		cd $(pwd)/$$subdir && make clean;\
		done


.PHONY:clean 

