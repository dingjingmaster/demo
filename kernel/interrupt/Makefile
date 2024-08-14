ifneq ($(KERNELRELEASE),)
name 	:= $(shell basename $(shell realpath -- $(shell pwd)))
obj-m 	+= intrpt.o
obj-m 	+= bottomhalf.o
else
PWD  := $(shell pwd)
KVER := $(shell uname -r)
KDIR := /usr/lib/modules/$(KVER)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	rm -rf *.o *.mod *.mod.c *.ko *.symvers *.order *.markers *~
endif
