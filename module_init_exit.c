/*
 * Sample program to demonstrate simple module init and exit
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init init_mod(void)
{
	printk(KERN_INFO "Hey, Sathish!\n");
	return 0;
}

static int __exit exit_mod(void)
{
	printk(KERN_INFO "Goodbye, Sathish\n");
}

module_init(init_mod);
module_exit(exit_mod);

MODULE_LICENSE("GPL");

