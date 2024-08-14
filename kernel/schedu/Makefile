ifneq ($(KERNELRELEASE),)
name 	:= $(shell basename $(shell realpath -- $(shell pwd)))
obj-m 	+= tasklet.o
tasklet-objs := example_tasklet.o
obj-m 	+= sched.o
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
