/*************************************************************************
> FileName: udev1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年11月18日 星期二 11时05分33秒
 ************************************************************************/
 #include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static int usb_event_notify(struct notifier_block *self,
                            unsigned long action, void *dev)
{
    struct usb_device *udev = dev;

    switch (action) {
        case USB_DEVICE_ADD:
            pr_info("USB device added: vid=%04x pid=%04x\n",
                    udev->descriptor.idVendor, udev->descriptor.idProduct);
            break;
        case USB_DEVICE_REMOVE:
            pr_info("USB device removed: vid=%04x pid=%04x\n",
                    udev->descriptor.idVendor, udev->descriptor.idProduct);
            break;
    }
    return NOTIFY_OK;
}

static struct notifier_block usb_nb = {
    .notifier_call = usb_event_notify,
};

static int __init usb_monitor_init(void)
{
    usb_register_notify(&usb_nb);
    pr_info("USB monitor loaded\n");
    return 0;
}

static void __exit usb_monitor_exit(void)
{
    usb_unregister_notify(&usb_nb);
    pr_info("USB monitor unloaded\n");
}

module_init(usb_monitor_init);
module_exit(usb_monitor_exit);
MODULE_LICENSE("GPL");
