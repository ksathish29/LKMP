/*
 * Implement a character driver
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "sathish_chdev"
#define CLASS_NAME "sathish_chdrv"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sathish Kumar");
MODULE_DESCRIPTION("Character driver in linux");

/* Meta information for the character driver */
static int major_number;
static int message[256] = {0};
static short msg_size;
static int num_opens = 0;
static struct class *cdrv_class = NULL;
static struct device *cdrv_dev = NULL;

/* Prototype for filesystem operations */
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static size_t dev_read(struct file *, char *, size_t, loff_t *);
static size_t dev_write(struct file *, const char *, size_t, loff_t *);

/* file_operations struct is defined in linux/fs.h. It provides a mechanism to register
 * call back functions to standard file operations. This character driver has read,
 * write, open and release functionalities
 */
static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

/* Kernel module init function */
static int __init chardrv_init(void)
{
	printk(KERN_INFO "%s: Initializing the character driver kernel module\n", __func__);

	/* Allocate major number for the device */
	major_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (major_number < 0) {
		printk(KERN_ALERT "Failed to register a major number for the device\n");
		return major_number;
	}

	printk(KERN_INFO "Registered the character device with major number %d\n", major_number);

	/* Register device class */
	cdrv_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(cdrv_class)) {
		printk(KERN_ALERT "Failed to create class for the device\n");
		goto class_create_fail;
	}

	printk(KERN_INFO "Device class registered\n");

	/* Register device driver */
	cdrv_dev = device_create(cdrv_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
	if (IS_ERR(cdrv_dev)) {
		printk(KERN_ALERT "Failed to create device\n");
		goto device_create_fail;
	}

	printk(KERN_INFO "Character device successfully registered\n");
	return 0;

device_create_fail:
	class_destroy(cdrv_class);
class_create_fail:
	unregister_chrdev(major_number, DEVICE_NAME);
	return -1;
}

/* Kernel module exit function */
static void __exit chardrv_exit(void)
{
	device_destroy(cdrv_class, MKDEV(major_number, 0));
	class_unregister(cdrv_class);
	class_destroy(cdrv_class);
	unregister_chrdev(major_number, DEVICE_NAME);
	printk(KERN_INFO "Removing character device kernel module\n");
}

/* dev_open is filesystem open operation that is called each time device is opened */
static int dev_open(struct inode *inodep, struct file *filep)
{
	num_opens++;
	printk(KERN_INFO "Character device has been opened %d times\n", num_opens);
	return 0;
}

/* dev_read is invoked when the userspace requests to read the file */
static size_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int err_count = 0;
	err_count = copy_to_user(buffer, message, msg_size);

	if (err_count == 0) {
		printk(KERN_INFO "Sent %d characters to the user\n", msg_size);
		return (msg_size = 0);
	} else {
		printk(KERN_ALERT "Faile to send characters to the user\n");
		return -EFAULT;
	}
}
		
/* dev_write is invoked when userspace writes into the file */
static size_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	sprintf(message, "%s(%zu letters)", buffer, len);
	msg_size = strlen(message);
	printk(KERN_INFO "Received %zu characters from the userspace\n", len);
	return len;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "Device released\n");
	return 0;
}

module_init(chardrv_init);
module_exit(chardrv_exit);
