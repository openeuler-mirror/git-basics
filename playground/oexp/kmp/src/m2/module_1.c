// Module that support for array parameter

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

#define MAX_LENGTH 7

static int my_array[MAX_LENGTH];
int array_num;

module_param_array(my_array, int, &array_num, 0644);
MODULE_PARM_DESC(my_array, "A integer array");
 
static int __init my_module_init(void) {
    int i;
    printk("The module 1 is loaded!\n");

    for(i = 0; i < array_num; i++) {
        printk("%d\n", my_array[i]);
	}

    return 0;
}

static void __exit my_module_exit(void) {
    printk("The module 1 is removed!\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
