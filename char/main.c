#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("Dual BSD/GPL");

static const char *device_name = "char";
static unsigned int nr_devices = 4;

static dev_t dev;
static struct cdev cdev;

static struct file_operations file_ops = {
	
};


static int hello_init(void)
{
	int err = alloc_chrdev_region(&dev, 0, nr_devices, device_name);
	if (err) {
		printk(KERN_ALERT "Cannot allocate dev region\n");
		return -1;
	}

	cdev_init(&cdev, &file_ops);
	cdev.owner = THIS_MODULE;

	err = cdev_add(&cdev, dev, nr_devices);
	if (err) {
		printk(KERN_ALERT "cdev_add() failed\n");
		return -1;
	}
		
	return 0;
}

static void hello_exit(void)
{
	unregister_chrdev_region(dev, nr_devices);
	cdev_del(&cdev);
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
