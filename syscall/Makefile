workdir=$(shell pwd)
subdirs= dlopen module

all:
	@for subdir in $(subdirs);\
		do\
		cd $(workdir)/$$subdir && make;\
		done

clean:
	@for subdir in $(subdirs);\
		do\
		cd $(workdir)/$$subdir && make clean;\
		done

.PHONY: clean all

