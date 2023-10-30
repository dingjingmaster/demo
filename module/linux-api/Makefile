# 注意 Module 名不要和 源码名字冲突

obj-m 		 	 = linuxAPI.o
linuxAPI-y 		:= main.o list-management.o hlist.o klist.o string.o

# -stdc90 不允许混合声明，且变量声明局限于文件开头或者符合语句开头
CFLAGS_MODULE 	:= -std=gnu99

KERNEL_BUILD 	:= /lib/modules/$(shell uname -r)/build

helloWorld:
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
