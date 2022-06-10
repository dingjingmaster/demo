/*************************************************************************
> FileName: udedv-hotplugin.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年06月25日 星期五 15时15分14秒
 ************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <libudev.h>
 
 
#undef asmlinkage
#ifdef __i386__
#define asmlinkage __attribute__((regparm(0)))
#else
#define asmlinkage 
#endif
 
 
static int udev_exit;
 
static void asmlinkage sig_handler(int signum)
{
	if (signum == SIGINT || signum == SIGTERM)
		udev_exit = 1;
}
 
 
static void print_device(struct udev_device *device, const char *source, int env)
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
 
	printf("%-6s[%llu.%06u] %-8s %s (%s)\n",
	       source,
	       (unsigned long long) tv.tv_sec, (unsigned int) tv.tv_usec,
	       udev_device_get_action(device),
	       udev_device_get_devpath(device),
	       udev_device_get_subsystem(device));
 
	if (env) {
		struct udev_list_entry *list_entry;
		udev_list_entry_foreach(list_entry, udev_device_get_properties_list_entry(device))
			printf("%s=%s\n",
			       udev_list_entry_get_name(list_entry),
			       udev_list_entry_get_value(list_entry));
		printf("\n");
	}
 
}
 
 
int udevadm_monitor(struct udev *udev)
{
	struct sigaction act;
	int env = 0;
	int print_kernel = 1;
	struct udev_monitor *kernel_monitor = NULL;
	fd_set readfds;
	int rc = 0;
 
	if (getuid() != 0) {
		fprintf(stderr, "root privileges needed to subscribe to kernel events\n");
		goto out;
	}
 
	/* set signal handlers */
	memset(&act, 0x00, sizeof(struct sigaction));
	act.sa_handler = (void (*)(int)) sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
 
	printf("monitor will print the received events.\n");
	
	if (print_kernel) {
		kernel_monitor = udev_monitor_new_from_netlink(udev, "udev"); //这里的udev源码中没有"udev"这个参数，不加进去返回值就为NULL，所以要加这个
		if (kernel_monitor == NULL) {
			rc = 3;
			printf("udev_monitor_new_from_netlink() error\n");
			goto out;
		}
 
		if (udev_monitor_enable_receiving(kernel_monitor) < 0) {
			rc = 4;
			goto out;
		}
 
		printf("UEVENT the kernel uevent: \n");
	}
	
	printf("\n");
	while (!udev_exit) {
		int fdcount;
		FD_ZERO(&readfds);
 
		if (kernel_monitor != NULL)
			FD_SET(udev_monitor_get_fd(kernel_monitor), &readfds);
 
		fdcount = select(udev_monitor_get_fd(kernel_monitor)+1, &readfds, NULL, NULL, NULL);
		if (fdcount < 0) {
			if (errno != EINTR)
				fprintf(stderr, "error receiving uevent message: %m\n");
			continue;
		}
 
		if ((kernel_monitor != NULL) && FD_ISSET(udev_monitor_get_fd(kernel_monitor), &readfds)) {
			struct udev_device *device;
			device = udev_monitor_receive_device(kernel_monitor);
			if (device == NULL)
				continue;
			print_device(device, "UEVENT", env);
			udev_device_unref(device);
		}
 
	}
 
out:
	udev_monitor_unref(kernel_monitor);
	return rc;
}
 
 
 
int main(int argc, char *argv[])
{
	struct udev *udev;
	int rc = 1;
 
	udev = udev_new();
	if (udev == NULL)
		goto out;
 
	udevadm_monitor(udev);
	goto out;
	rc = 2;
 
out:
	udev_unref(udev);
	return rc;
}

