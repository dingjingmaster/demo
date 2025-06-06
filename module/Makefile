obj-m = \
	io.o \
	kbd.o \
	list1.o \
	list2.o \
	timer.o \
	hidden.o \
	device.o \
	memory.o \
	kprobes.o \
	cmd-mod.o \
	locking1.o \
	locking2.o \
	list-proc.o \
	list-full.o \
	list-sync.o \
	sysfs.o \
	sched-spin.o \
	hello-world.o \

#do-mmap.o \


CFLAGS_MODULE := -std=gnu99
KERNEL_BUILD := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNEL_BUILD) M=$(shell pwd) modules

clean:
	@rm -f *.o
	@rm -f *.ko
	@rm -f *.mod
	@rm -f .*.cmd
	@rm -f .*.mod
	@rm -f *.order
	@rm -f *.mod.c
	@rm -f *.symvers
	@rm -rf .tmp_versions

#all : subdirs

#subdirs:
#	@for n in $(SUBDIRS); do $(MAKE) -C $$n || exit 1; done

#clean:
#	@for n in $(SUBDIRS); do $(MAKE) -C $$n clean; done

