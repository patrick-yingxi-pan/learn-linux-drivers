#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

static dev_t dev;
static struct cdev cdev;
static const char *device_name = "char";
static unsigned int nr_devices = 4;

int char_open(struct inode *inode, struct file *filp)
{
	printk("opened with minor number: %d\n", iminor(inode));
	filp->private_data = (void *)(long)(iminor(inode) + '0');
	return 0;
}

int char_release(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t char_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	char *p;
	int err;
	ssize_t retval = 0;

	printk("char_read(%zu)\n", count);
	p = kmalloc(count, GFP_KERNEL);
	if (!p) {
		printk("read() error: no memory left\n");
		return -ENOMEM;
	}

	memset(p, (long)filp->private_data, count);
	err = copy_to_user(buff, p, count);
	if (err)
		retval = -EFAULT;

	kfree(p);
	return count;
}

ssize_t char_write(struct file *filp, const char __user *buff,
	size_t count, loff_t *offp)
{
	return 0;
}

static struct file_operations file_ops = {
	.open = char_open,
	.release = char_release,
	.read = char_read
};


static int char_init(void)
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

static void char_exit(void)
{
	unregister_chrdev_region(dev, nr_devices);
	cdev_del(&cdev);
	printk(KERN_ALERT "%s.ko removed\n", device_name);
}

module_init(char_init);
module_exit(char_exit);
