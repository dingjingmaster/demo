obj-m 			 = ramfs-mini.o
ramfs-mini-y    += inode.o file-nommu.o

KERNEL_BUILD 	:= /lib/modules/$(shell uname -r)/build

helloWorld:
	$(MAKE) -C $(KERNEL_BUILD) M=$(shell pwd) modules

clean:
	@rm -f *.o
	@rm -f .*.o
	@rm -f *.ko
	@rm -f *.mod
	@rm -f .*.o.d
	@rm -f .*.cmd
	@rm -f .*.mod
	@rm -f *.order
	@rm -f *.mod.c
	@rm -f *.symvers
	@rm -rf .tmp_versions
