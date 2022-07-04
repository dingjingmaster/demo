subdirs= dlopen

all:
	@for subdir in $(subdirs);\
		do\
		cd $$subdir && make;\
		done

clean:
	@for subdir in $(subdirs);\
		do\
		cd $$subdir && make clean;\
		done

.PHONY: clean all

