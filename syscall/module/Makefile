obj-m := syscall.o

PWD := $(shell pwd)

KERNELDIR := /lib/modules/$(shell uname -r)/build

EXTRA_CFLAGS = -O0


all:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc -o test.run $(PWD)/test.c

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f $(PWD)/*.run
