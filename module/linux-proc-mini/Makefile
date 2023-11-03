obj-m   			+= proc-mini.o

CFLAGS_task_mmu.o	+= $(call cc-option,-Wno-override-init,)
#proc-mini-m			:= nommu.o task_nommu.o
#proc-$(CONFIG_MMU)	:= task_mmu.o
proc-mini-m       	+= inode.o root.o base.o generic.o array.o fd.o
proc-mini-m 		+= cmdline.o

KERNEL_BUILD 		:= /lib/modules/$(shell uname -r)/build

#proc-$(CONFIG_TTY)      += proc_tty.o
#proc-y	+= consoles.o
#proc-y	+= cpuinfo.o
#proc-y	+= devices.o
#proc-y	+= interrupts.o
#proc-y	+= loadavg.o
#proc-y	+= meminfo.o
#proc-y	+= stat.o
#proc-y	+= uptime.o
#proc-y	+= util.o
#proc-y	+= version.o
#proc-y	+= softirqs.o
#proc-y	+= namespaces.o
#proc-y	+= self.o
#proc-y	+= thread_self.o
#proc-$(CONFIG_PROC_SYSCTL)	+= proc_sysctl.o
#proc-$(CONFIG_NET)		+= proc_net.o
#proc-$(CONFIG_PROC_KCORE)	+= kcore.o
#proc-$(CONFIG_PROC_VMCORE)	+= vmcore.o
#proc-$(CONFIG_PRINTK)	+= kmsg.o
#proc-$(CONFIG_PROC_PAGE_MONITOR)	+= page.o
#proc-$(CONFIG_BOOT_CONFIG)	+= bootconfig.o

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
