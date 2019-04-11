/*********************************************************************************************
 *                      FILENAME: timer_lkm.c
 * *****************************************************************************************/

/*
 * @Title: Linux kernel module in which timer fires at periodic rate determined by user
 * @Author: Vikrant Waje
 * @Date: 10 February 2019
 * @Tools: buildroot, vim,gcc
 */

/*********************************************************************************
 *                        HEADER FILE SECTION
 * **********************************************************************************/

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/timer.h>
#include<linux/jiffies.h>

/*********************************************************************************
 *                        LICENSING INFORMATION SECTION
 * **********************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("VIKRANT WAJE");
MODULE_DESCRIPTION("TIMER INTERRUPT KERNEL MDOULE");
MODULE_VERSION("1.0");

/*********************************************************************************
 *                       GLOBAL SECTION
 * **********************************************************************************/
struct timer_list my_timer_t;
static char *my_name = "Vikrant";
static int time_period = 0.5;

/*********************************************************************************
 *                        MODULE PARAMETER SECTION
 * **********************************************************************************/
module_param(my_name,charp,0);
module_param(time_period,int,0);

MODULE_PARM_DESC(my_name,"This is the name that will appear every (default)500 msec");
MODULE_PARM_DESC(time_period,"This is the period after which timer fires");

/*******************************************************************************
 * @Name: timer callback function
 * @brief: Fires every periodic rate
 * @param: data = data to be passed to callback function
 * @return: NONE
 ******************************************************************************/

void timer_callback(unsigned long data){
	static int timer_count = 0;
	timer_count++;				//Increment the static count variable
	printk(KERN_DEFAULT"My name is %s",my_name);
	printk(KERN_DEFAULT"Timer called %d times",timer_count);
	mod_timer(&my_timer_t, jiffies + ((HZ*(time_period)))); //Reset the timer to fire periodically


}

/*******************************************************************************
 * @Name: custom timer initialise function
 * @brief: Responsible for setting the expiry time, callback function and data 
 * @param: my_timer_t: structure that contains all the timer related parameter
 * @return: NONE
 ******************************************************************************/
static void custom_timer_init(struct timer_list *my_timer_t){

my_timer_t->function = timer_callback;
my_timer_t->data = 0;
my_timer_t->expires = jiffies + ((HZ*(time_period)));

}
/*******************************************************************************
 * @Name: Entry function for timer kernel module
 * @brief: Invoked when we use insmod
 * @param: NONE
 * @return: static int: return status
 ******************************************************************************/
static int __init timer_start(void){
	
	printk(KERN_DEFAULT"TIMER INTERRUPT KERNEL MODULE INITIALISATION FUNCTION");
	init_timer(&my_timer_t);
	custom_timer_init(&my_timer_t);
	printk(KERN_DEFAULT"TIMER SUCCESSFULLY INITIALISED");

	add_timer(&my_timer_t);

	return 0;

}

/*******************************************************************************
 * @Name: Exit function for timer kernel module
 * @brief: Invoked when we use rmmod
 * @param: NONE
 * @return: static int: return status
 ******************************************************************************/
static void __exit timer_end(void){
	printk(KERN_DEFAULT"TIMER INTERRUPT KERNEL MODULE EXIT FUNCTION");
	del_timer(&my_timer_t);
}

module_init(timer_start);
module_exit(timer_end);
