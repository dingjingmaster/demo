obj-m = device.o
KERNEL_BUILD := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL_BUILD) M=$(shell pwd) modules

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
