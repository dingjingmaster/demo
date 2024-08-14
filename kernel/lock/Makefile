ifneq ($(KERNELRELEASE),)
name 	:= $(shell basename $(shell realpath -- $(shell pwd)))
obj-m 	        += mutex.o
mutex-objs      += example_mutex.o
obj-m           += spin-lock.o
spin-lock-objs 	+= example_spinlock.o
obj-m           += rw-lock.o
rw-lock-objs    += example_rwlock.o
obj-m           += atomic.o
atomic-objs     += example_atomic.o
#module1-objs := tiny-fs.o #$(wildcard $(shell pwd)/*.c)
#$(shell basename $(shell realpath -- $(shell pwd))).o
else
PWD  := $(shell pwd)
KVER := $(shell uname -r)
KDIR := /usr/lib/modules/$(KVER)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	rm -rf *.o *.mod *.mod.c *.ko *.symvers *.order *.markers *~
endif
