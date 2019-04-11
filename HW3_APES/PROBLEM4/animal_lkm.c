/*********************************************************************************************
 *                      FILENAME: animal_lkm.c
 * *****************************************************************************************/

/*
 * @Title: Linux kernel module in animal ecosystem is input and we have to create a linked list in 2 sets
 Set 1: COntains only single occurence node
 Set 2: Create a linked list according to filter
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
#include<linux/list.h>
#include<linux/slab.h>
#include<linux/time.h>
/*********************************************************************************
 *                        LICENSING INFORMATION SECTION
 * **********************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("VIKRANT WAJE");
MODULE_DESCRIPTION("ANIMAL ECOSYSTEM KERNEL MODULE");
MODULE_VERSION("1.0");

/*********************************************************************************
 *                       GLOBAL SECTION
 * **********************************************************************************/
typedef struct animal_ecosystem{
	char animal_name[15];
	int count;
	struct list_head list;

}animal_t;

static char *animal_find_name = NULL;
static int find_count = 0;

int total_nodes =0;		// Count total number of nodes
int no_nodes = 0;
int no_count_nodes =0;
int no_type_nodes =0;
int no_both_nodes =0;
int no_no_nodes =0;
struct timespec tstart,tend;		// Structure for measuring time
double elapsedtime;
struct list_head root;		//Head of type animal_t which is a struct
struct list_head count_root;	//This linked list will store all the ecosystem that has count greater than specified value
struct list_head type_root;	//This linked list will store all the ecosystem that has given animal type
struct list_head both_root;	//This linked list is to be created when both filters are enabled
struct list_head no_root;	// This linked list is to be created when both filters are disabled

/*********************************************************************************
 *                        MODULE PARAMETER SECTION
 * **********************************************************************************/
module_param(animal_find_name,charp,0);
module_param(find_count,int,0);
//module_param(animal_array,int,0);
MODULE_PARM_DESC(animal_find_name,"This is the name that should be searched in the linked list");
MODULE_PARM_DESC(find_count,"This is the count variable");

/*******************************************************************************
 * @Name: Entry function for Animal Ecosystem kernel module
 * @brief: Invoked when we use insmod
 * @param: NONE
 * @return: static int: return status
 ******************************************************************************/
static int __init animal_ecosystem_start(void){

	getnstimeofday(&tstart);
	printk(KERN_DEFAULT"\n---------------------------ANIMAL ECOSYSTEM KERNEL MODULE INITIALISATION FUNCTION STARTS-------------------------");
	int i =0;
	int j=0;
	int count_animal =0;
	animal_t *new_node = kmalloc(sizeof(animal_t),GFP_KERNEL);
	animal_t *ptr;
	int status = 0;
	// Name of animals
	char names_t[50][15] = {"Tiger","Dog","Cat","Shark","Dolphin",\
		"Parrot","Seal","Piranha","Monkey","Squirrel",\
			"Elephant","Lion","Donkey","Sheep","Chicken",\
			"Rat","Goat","Frog","Deer","Yak",\
			"Whale","Snake","Crocodile","Moose","Goose",\
			"Camel","Penguin","Pig","Lizard","Ant",\
			"Bee","Bear","Horse","Cow","Bull",\
			"Dog","Monkey","Tiger","Bull","Goat",\
			"Whale","Rat","Lion","Deer","Yak",\
			"Dog","Tiger","Sheep","Chicken","Frog"};
	char temp[15];
	printk(KERN_DEFAULT"\n--------------INPUT SEED ARRAY--------------------------------------");
	for(i=0;i<50;i++){
		printk(KERN_DEFAULT"%s",names_t[i]);

	}

	printk("\n----------SORTED LIST OF ANIMAL IS AS FOLLOWS:---------------------------");

	for(i=0;i<50;i++){
		for(j=i+1;j<50;j++){
			status = strcmp(names_t[i],names_t[j]);
			if(status >0){
				strcpy(temp,names_t[i]);
				strcpy(names_t[i],names_t[j]);
				strcpy(names_t[j],temp);
			}
		}	

	}

	for(i=0;i<50;i++){
		printk(KERN_DEFAULT"%s",names_t[i]);

	}


	INIT_LIST_HEAD(&root);	//Make it as root for set1
	INIT_LIST_HEAD(&count_root);	//Root of set2: Count linked list
	INIT_LIST_HEAD(&type_root);	//Root of set2: type linkedlist
	INIT_LIST_HEAD(&both_root);	//Root of set2: When both filters are enabled
	INIT_LIST_HEAD(&no_root);	// Root of set2: When no filters are enabled
	i=0;


	// SELECTING UNIQUE ANIMAL TYPE FOR LINKED LIST CREATION
	while(i<50){
		for(j=i;j<50;j++){
			if(strcmp(names_t[i],names_t[j])==0){
				count_animal++;
			}
		}
		no_nodes++;
		animal_t *new_node = kmalloc(sizeof(animal_t),GFP_KERNEL);
		strcpy(new_node->animal_name,names_t[i]);
		new_node->count =count_animal;
		INIT_LIST_HEAD(&new_node->list);
		list_add_tail(&new_node->list,& (root));
		i=i+count_animal;
		count_animal=0;
	}

	printk(KERN_DEFAULT"\n--------------------SET1: LINKED LIST WITH UNIQUE ELEMENTS--------------------------------------");

	list_for_each_entry(ptr,&root,list){
		printk(KERN_DEFAULT"Name of animal:%s, Count:%d",ptr->animal_name,ptr->count);
	}


	/*-----------------------------------------SET2------------------------------------------------------------------------*/
	//CODE FOR EXTRACTING ELEMENTS THAT SATISFY BOTH FILTER OPTION

	if(find_count !=0 && animal_find_name!=NULL){
		list_for_each_entry(ptr,&root,list){
			if(ptr->count >=find_count && (strcmp(ptr->animal_name,animal_find_name)==0)){
				animal_t *new_node = kmalloc(sizeof(animal_t),GFP_KERNEL);
				strcpy(new_node->animal_name,ptr->animal_name);
				new_node->count=ptr->count;
				INIT_LIST_HEAD(&new_node->list);
				list_add_tail(&new_node->list,&both_root);

			}

		}
		printk(KERN_DEFAULT"\n-----------------SET2:NODES WHEN BOTH FILTER ARE ENABLED------------------------------------");
		list_for_each_entry(ptr,&both_root,list){
			no_both_nodes++;
			printk(KERN_DEFAULT"Name of animal in count_list:%s, Count:%d",ptr->animal_name,ptr->count);

		}	
	}

	else if(find_count !=0 || animal_find_name!=NULL){
		// CODE FOR EXTRACTING ELEMENTS WHEN ONLY COUNT FILTER IS ENABLED
		if(find_count!=0){
			list_for_each_entry(ptr,&root,list){
				if(ptr->count >=find_count){
					animal_t *new_node = kmalloc(sizeof(animal_t),GFP_KERNEL);
					strcpy(new_node->animal_name,ptr->animal_name);
					new_node->count=ptr->count;
					INIT_LIST_HEAD(&new_node->list);
					list_add_tail(&new_node->list,&count_root);

				}
			}
			printk(KERN_DEFAULT"\n---------------------------SET2:NODES WHEN COUNT FILTER IS ENABLED----------------------------------------");
			list_for_each_entry(ptr,&count_root,list){
				no_count_nodes++;
				printk(KERN_DEFAULT"Name of animal in count_list:%s, Count:%d",ptr->animal_name,ptr->count);

			}
		}

		//CODE FOR EXTRACTING ELEMENTS THAT HAS GIVEN TYPE OF ANIMAL
		if(animal_find_name !=NULL){
			list_for_each_entry(ptr,&root,list){
				if(strcmp(ptr->animal_name,animal_find_name)==0){
					animal_t *new_node = kmalloc(sizeof(animal_t),GFP_KERNEL);
					strcpy(new_node->animal_name,ptr->animal_name);
					new_node->count=ptr->count;
					INIT_LIST_HEAD(&new_node->list);
					list_add_tail(&new_node->list,&type_root);

				}
			}
			printk(KERN_DEFAULT"\n--------------------------SET2: NODES WHEN TYPE FILTER IS ENABLED----------------------------------------------");
			list_for_each_entry(ptr,&type_root,list){
				no_type_nodes++;
				printk(KERN_DEFAULT"Name of animal in type_list:%s, Count:%d",ptr->animal_name,ptr->count);

			}	

		}

	}
	//CODE FOR PRINTING NODES WHEN NO FILTER ARE GIVEN
	else {

		list_for_each_entry(ptr,&root,list){
		
				animal_t *new_node = kmalloc(sizeof(animal_t),GFP_KERNEL);
				strcpy(new_node->animal_name,ptr->animal_name);
				new_node->count=ptr->count;
				INIT_LIST_HEAD(&new_node->list);
				list_add_tail(&new_node->list,&no_root);

			

		}		
		printk(KERN_DEFAULT"\n-----------------SET2:NODES WHEN BOTH FILTER ARE DISABLED------------------------------------");
		list_for_each_entry(ptr,&no_root,list){

			printk(KERN_DEFAULT"Name of animal in count_list:%s, Count:%d",ptr->animal_name,ptr->count);
			no_no_nodes++;
		}	

	}


	printk(KERN_DEFAULT"\n--------------------------TOTAL NODES ALLOCATION----------------------------------------------------------");
	printk(KERN_DEFAULT" \nTotal nodes allocated for original sorted linked list = %d",no_nodes);
	total_nodes +=no_nodes;
	if(find_count !=0 && animal_find_name !=NULL){
		printk(KERN_DEFAULT"\n Total nodes allocated when both filter are enabled = %d", no_both_nodes);
		total_nodes +=no_both_nodes;
	}


	else if(find_count!=0 || animal_find_name!=NULL){
		if(find_count!=0){
			printk(KERN_DEFAULT"\n Total nodes allocated when count filter is enabled = %d",no_count_nodes);
			total_nodes +=no_count_nodes;
		}
		if(animal_find_name != NULL){
			printk(KERN_DEFAULT"\n Total nodes allocated when type filter is enabled = %d",no_type_nodes);
			total_nodes +=no_type_nodes;
		}
	}
	else{
		printk(KERN_DEFAULT"\n Total nodes allocated when both filter are disabled = %d", no_no_nodes);
		total_nodes +=no_no_nodes;
	}
	printk("\n Total nodes allocated = %d and total size that was allocated on heap = %ld bytes",total_nodes, (total_nodes * sizeof(animal_t)));
	getnstimeofday(&tend);
	//	elapsedtime = tend.tv_sec ; 	//seconds to msec
	printk(KERN_DEFAULT"\n Time required to insert module is %lu nsec",(tend.tv_nsec - tstart.tv_nsec));
	printk(KERN_DEFAULT"\n---------------------------ANIMAL ECOSYSTEM KERNEL MODULE INITIALISATION FUNCTION ENDS-------------------------");
	return 0;

}
/*******************************************************************************
 * @Name: Exit function for Animal Ecosystem kernel module
 * @brief: Invoked when we use rmmod
 * @param: NONE
 * @return: static int: return status
 ******************************************************************************/
static void __exit animal_ecosystem_end(void){
	getnstimeofday(&tstart);
	animal_t *ptr,*tmp;
	printk(KERN_DEFAULT"\n-------------------------ANIMAL ECOSYSTEM  KERNEL MODULE EXIT FUNCTION STARTS---------------------------");
	printk(KERN_DEFAULT"\n-------------------------SET1:DELETING THE NODES OF ORIGINAL SORTED LINKED LIST-----------------------");
	list_for_each_entry_safe(ptr,tmp,&root,list){
		printk(KERN_DEFAULT"\nfreeing node %s count = %d",ptr->animal_name,ptr->count);
		list_del(&ptr->list);
		kfree(ptr);
	}

	if(find_count!=0 && animal_find_name!=NULL){
		printk(KERN_DEFAULT"\n--------------SET2:DELETING NODES WHEN BOTH FILTER WHEN ENABLED----------------------------------");
		list_for_each_entry_safe(ptr,tmp,&both_root,list){
			printk(KERN_DEFAULT"\nfreeing node %s, count = %d",ptr->animal_name,ptr->count);
			list_del(&ptr->list);
			kfree(ptr);
		}
	}



	else if(find_count!=0 || animal_find_name!=NULL){
		if(find_count!=0){
			printk(KERN_DEFAULT"\n------------------------------SET2:DELETING NODES IN COUNT TYPE-----------------------------");
			list_for_each_entry_safe(ptr,tmp,&count_root,list){
				printk(KERN_DEFAULT"\nfreeing node %s, count= %d",ptr->animal_name,ptr->count);
				list_del(&ptr->list);
				kfree(ptr);
			}
		}


		if(animal_find_name !=NULL){
			printk(KERN_DEFAULT"\n-----------------------------SET2:DELETING NODES IN NAME TYPE--------------------------------");
			list_for_each_entry_safe(ptr,tmp,&type_root,list){
				printk(KERN_DEFAULT"\nfreeing node %s",ptr->animal_name);
				list_del(&ptr->list);
				kfree(ptr);
			}		
		}
	
	}
	else{
		printk(KERN_DEFAULT"\n--------------SET2:DELETING NODES WHEN BOTH FILTER ARE DISABLED----------------------------------");
		list_for_each_entry_safe(ptr,tmp,&no_root,list){
			printk(KERN_DEFAULT"\nfreeing node %s, count = %d",ptr->animal_name,ptr->count);
			list_del(&ptr->list);
			kfree(ptr);
		}
	}
	printk(KERN_DEFAULT"\n--------------------------TOTAL NODES DEALLOCATION----------------------------------------------------------");
	printk(KERN_DEFAULT" \nTotal nodes deallocated for original sorted linked list = %d",no_nodes);

	if(find_count !=0 && animal_find_name !=NULL){
		printk(KERN_DEFAULT"\n Total nodes deallocated when both filter are enabled = %d", no_both_nodes);

	}

	else if(find_count !=0 || animal_find_name!=NULL){
		if(find_count!=0){
			printk(KERN_DEFAULT"\n Total nodes deallocated when count filter is enabled = %d",no_count_nodes);

		}
		if(animal_find_name != NULL){
			printk(KERN_DEFAULT"\n Total nodes deallocated when type filter is enabled = %d",no_type_nodes);

		}
	}

	else{
		printk(KERN_DEFAULT"\n Total nodes deallocated when both filter are disabled = %d", no_no_nodes);

	}	printk("\n Total nodes deallocated = %d and total size that was deallocated on heap = %ld bytes",total_nodes, (total_nodes * sizeof(animal_t)));

	getnstimeofday(&tend);
	//	elapsedtime=(tend.tv_sec - tstart.tv_sec)*1000.0;	//seconds to msec
	printk(KERN_DEFAULT"\n Time required to remove module is %lu nsec",tend.tv_nsec - tstart.tv_nsec);
	printk(KERN_DEFAULT"\n-------------------------ANIMAL ECOSYSTEM  KERNEL MODULE EXIT FUNCTION ENDS---------------------------");
}

module_init(animal_ecosystem_start);
module_exit(animal_ecosystem_end);
