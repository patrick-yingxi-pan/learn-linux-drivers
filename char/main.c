#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

static dev_t dev;
static const char *device_name = "char";
static unsigned int nr_devices = 4;

static int hello_init(void)
{
	int r = alloc_chrdev_region(&dev, 0, nr_devices, device_name);
	if (r != 0) {
		printk(KERN_ALERT "Cannot allocate dev region\n");
		return -1;
	} else {
		printk(KERN_ALERT "Dev region allocated\n");
	}
	printk(KERN_ALERT "Hello, world\n");
	return 0;
}

static void hello_exit(void)
{
	unregister_chrdev_region(dev, nr_devices);
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
