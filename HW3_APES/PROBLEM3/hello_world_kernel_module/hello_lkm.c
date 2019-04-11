#include<linux/kernel.h>
#include<linux/module.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("VIKRANT WAJE");
MODULE_DESCRIPTION("Hello world");
MODULE_VERSION("1.0");


static int __init hello_start(void){
	
	printk(KERN_DEBUG"HELLO VIKRANT, KERNEL MODULE HERE");
	return 0;

}

static void __exit hello_end(void){
	printk(KERN_DEBUG"exiting hello world module vikrant");

}

module_init(hello_start);
module_exit(hello_end);
