#obj-m = \
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
	simplefs.o \
	sched-spin.o \
	hello-world.o \
	work-queue.o \
	notifier.o \
	notifier1.o \
	ftrace.o \
	ftrace3.o \
	ftrace1.o \
	rht-rcu.o \
	work-queue.o \
	udev.o \
	udev2.o \
	arm-openat.o \
	udev1.o

obj-m = \
	arm64-hook1.o \

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

