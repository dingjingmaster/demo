pwd=$(shell pwd)

ifeq ($(shell uname -o), Darwin)
	subdirs = object-c 
else ifeq ($(shell uname -o), GNU/Linux)
	subdirs = \
		version \
		hello \
		start-stop \
		chardev \
		procfs \
		sysfs \
		ioctl \
		syscall \
		sleep \
		completions \
		lock \
		x \
		schedu \
		interrupt \
		virt \
		device-model \
		optimizations \

endif

# learn

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
