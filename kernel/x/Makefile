ifneq ($(KERNELRELEASE),)
name 	:= $(shell basename $(shell realpath -- $(shell pwd)))
obj-m    += print.o
print-m  += print_string.o

obj-m    += kbleds.o
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
