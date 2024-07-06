subdirs = clone-execve

all:
	@for subdir in $(subdirs); \
		do \
		cd $(shell pwd)/$$subdir && make; \
		done

clean:
	@for subdir in $(subdirs);\
		do\
		cd $(shell pwd)/$$subdir && make clean;\
		done


.PHONY:clean all 
