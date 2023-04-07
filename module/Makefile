obj-m = hello-world.o
KERNEL_BUILD := /lib/modules/$(shell uname -r)/build

subdirs = helloWorld device

all:
	@for subdir in $(subdirs); \
		do \
			cd $(shell pwd)/$$subdir && make; \
		done

clean:
	@for subdir in $(subdirs); \
		do \
			cd $(shell pwd)/$$subdir && make clean; \
		done
