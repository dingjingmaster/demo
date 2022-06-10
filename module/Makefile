obj-m = hello-world.o
KERNEL_BUILD := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL_BUILD) M=$(shell pwd) modules

clean:
	rm -f *.o
	rm -f *.ko
	rm -f *.order
	rm -f *.mod.c
	rm -f *.symvers
