
/*********************************************************************************************
 *			FILENAME: my_sort.c
 * *****************************************************************************************/

/*
 * @Title:Sorting numbers in buffer in descending order using custom system call my_sort
 * @Author: Vikrant Waje
 * @Date: 10 February 2019
 * @Tools: buildroot, vim,gcc
 */

/*********************************************************************************
 *                        HEADER FILE SECTION
 * **********************************************************************************/

#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<stdlib.h>
#include<time.h>
#include<stdint.h>

/***************************************************************************************
 *			MAIN FUNCTION
 * **************************************************************************************/
/*
 * @Name: main
 * @brief: SYstem call at number 399 is invoked 
 *  ------>syscall(399): User defined function for sorting numbers in descending order
 */
void main(){

	//Sorting system call
	long int stat =-1;
	size_t buffer_size =256;
	uint32_t  i=0;
	int32_t *src_buffer =(int32_t *)malloc(sizeof(int32_t)*buffer_size);
	if(src_buffer == NULL){
		printf("\n\r Source buffer not allocated successfully");
	}

	int32_t *sorted_buffer = (int32_t *)malloc(sizeof(int32_t)*buffer_size);
	if(sorted_buffer == NULL){
		printf("\n\rSorted buffer not allocated successfully");
	}

	printf("\n\rBuffer size = %lu",(unsigned long)buffer_size);


	printf("\n\rInput buffer content is ");
	for(i=0;i<buffer_size;i++){
		if(i%8==0)
			printf("\n\r");
		*(src_buffer + i) = rand()%1024;		//Divide by 1024 so that everything will be within range of 0-1024
		printf("\t%ld",*(src_buffer +i));
	}


	stat =syscall(399,src_buffer,buffer_size,sorted_buffer);	//System call to invoke sorting function

	if(stat ==0){
		printf("\n\r  Successfully invoked sort system call");

	}
	else{
		printf("sort System call not invoked properly");

	}


	printf("\n\rSorted buffer content is: ");
	for(i=0;i<buffer_size;i++){
		if(i%8==0)
			printf("\n\r");
		printf("\t%ld",*(sorted_buffer +i));
	}


}
