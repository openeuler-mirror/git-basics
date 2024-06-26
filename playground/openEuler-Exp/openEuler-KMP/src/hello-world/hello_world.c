/* Hello from Kernel! */

#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h>

static char* guy = "Dinu";
module_param(guy, charp, 0644);
MODULE_PARM_DESC(guy, "char* param\n");

static int year = 2013;
module_param(year, int, 0644);
MODULE_PARM_DESC(year, "int param\n");

void hello_world(char* guy, int year) {
	printk(KERN_ALERT "Hello, %s, %d!\n", guy, year);
}

int __init hello_init(void) {
	printk(KERN_ALERT "Init module.\n");
	hello_world(guy, year);
	
	return 0;
}

void __exit hello_exit(void) {
	printk(KERN_ALERT "Exit module.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
