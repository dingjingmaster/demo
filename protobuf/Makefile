pwd=$(shell pwd)

subdirs= c++

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

