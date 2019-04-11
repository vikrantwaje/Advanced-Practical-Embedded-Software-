
/*********************************************************************************************
 *			FILENAME: regression_test.c
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
 * @param: NONE
 * @return: NONE
 */

void main(){
	/************************************************************************************************************************************************************
	 *									
	 *								TEST CASE 0
	 *				DESCRIPTION: When user does not have any error checking functionality in user application
	 *				PARAMETER 1: INPUT UNSORTED BUFFER IS ON HEAP
	 *				PARAMETER 2: OUTPUT SORTED BUFFER IS NULL 
	 *				PARAMETER 3: SIZE IS 256
	 *				EXPECTED RESULT: FAIL SINCE UNSORTED BUFFER IS NULL(RETURN ENOSPC FROM KERNEL)
	 *
	 * **********************************************************************************************************************************************/
	//Sorting system call
	long int stat =-1;
	long int stat_var=0;
	size_t buffer_size =256;
	size_t i=0;


	//Source buffer
	int32_t *src_buffer = (int32_t *)malloc(sizeof(int32_t) *buffer_size);


	//Sorted buffer

	int32_t *sorted_buffer = NULL;
	
	//Print buffer size

	printf("\n\rBuffer size = %lu",(unsigned long)buffer_size);



	//Print contents of input buffer
	printf("\n\rInput buffer content is ");
	for(i=0;i<buffer_size;i++){
		*(src_buffer + i) = rand()%1024;		//Divide by 1024 so that everything will be within range of 0-1024
		printf("\t%ld",*(src_buffer +i));
	}


	stat =syscall(399,src_buffer,buffer_size,sorted_buffer);	//System call to invoke sorting function

	//Error handling
	if(stat !=0){
		printf("\n\r  Successfully invoked sort system call");

	
	}
	else{
		printf("sort System call not invoked properly.SOurce or sorted buffer is NULL");
		stat_var= -1;

	}

	//Print contents of sorted buffer
	printf("\n\rSorted buffer content is: ");
	for(i=0;i<buffer_size;i++)
		printf("\t%ld",*(sorted_buffer +i));

	free(sorted_buffer);
	free(src_buffer);
	if(stat_var == 0){
		printf("\n\rTEST 0 PASSED SUCCESSFULLY");
	}
	else{
		printf("\n\rTEST 0 FAILED");
	}

	/************************************************************************************************************************************************************
	 *									
	 *								TEST CASE 1
	 *				DESCRIPTION: When user does not have any error checking functionality in user application
	 *				PARAMETER 1: INPUT UNSORTED BUFFER IS ON HEAP
	 *				PARAMETER 2: OUTPUT SORTED BUFFER IS ON HEAP
	 *				PARAMETER 3: SIZE IS LESS THAN 255
	 *				EXPECTED RESULT: FAIL SINCE UNSORTED BUFFER IS NULL(RETURN EINVAL FROM KERNEL)
	 *
	 * **********************************************************************************************************************************************/
	//Sorting system call
	stat =-1;
	stat_var=0;
	buffer_size =5;
	i=0;


	//Source buffer
	src_buffer = (int32_t *)malloc(sizeof(int32_t) *buffer_size);


	//Sorted buffer

	sorted_buffer = (int32_t *)malloc(sizeof(int32_t)*buffer_size);
	
	//Print buffer size

	printf("\n\rBuffer size = %lu",(unsigned long)buffer_size);



	//Print contents of input buffer
	printf("\n\rInput buffer content is ");
	for(i=0;i<buffer_size;i++){
		*(src_buffer + i) = rand()%1024;		//Divide by 1024 so that everything will be within range of 0-1024
		printf("\t%ld",*(src_buffer +i));
	}


	stat =syscall(399,src_buffer,buffer_size,sorted_buffer);	//System call to invoke sorting function

	//Error handling
	if(stat !=0){
		printf("\n\r  Successfully invoked sort system call");

	
	}
	else{
		printf("sort System call not invoked properly.Buffersize is less");
		stat_var= -1;

	}

	//Print contents of sorted buffer
	printf("\n\rSorted buffer content is: ");
	for(i=0;i<buffer_size;i++)
		printf("\t%ld",*(sorted_buffer +i));

	free(sorted_buffer);
	free(src_buffer);
	if(stat_var == 0){
		printf("\n\rTEST 1 PASSED SUCCESSFULLY");
	}
	else{
		printf("\n\rTEST 1 FAILED");
	}
	/************************************************************************************************************************************************************
	 *
	 *								TEST CASE 2
	 *				DESCRIPTION:
	 *				PARAMETER 1: INPUT UNSORTED BUFFER IS ON HEAP AND CONTAINS ALL NUMBERS(INTEGERS) GENERATED by rand() function
	 *				PARAMETER 2: OUTPUT SORTED BUFFER IS ON HEAP 
	 *				PARAMETER 3: SIZE IS POSITIVE AND GREATER THAN OR EQUAL TO 256
	 *				EXPECTED RESULT: PASS
	 *
	 * **********************************************************************************************************************************************/
	//Sorting system call
	 stat =-1;
	 buffer_size =256;
	 stat_var =0;
	if(buffer_size<0){
		printf("\n\rSize is negative");
		stat_var =-1;
	}
	 i=0;


	//Source buffer
	src_buffer =(int32_t *)malloc(sizeof(int32_t)*buffer_size);
	if(src_buffer == NULL){
		printf("\n\r Source buffer not allocated successfully");
		stat_var = -1;
	}

	//Sorted buffer

	sorted_buffer = (int32_t *)malloc(sizeof(int32_t)*buffer_size);
	if(sorted_buffer == NULL){
		printf("\n\rSorted buffer not allocated successfully");
		stat_var =-1;
	}
	//Print buffer size

	printf("\n\rBuffer size = %lu",(unsigned long)buffer_size);



	//Print contents of input buffer
	printf("\n\rInput buffer content is ");
	for(i=0;i<buffer_size;i++){
		*(src_buffer + i) = rand()%1024;		//Divide by 1024 so that everything will be within range of 0-1024
		printf("\t%ld",*(src_buffer +i));
	}


	stat =syscall(399,src_buffer,buffer_size,sorted_buffer);	//System call to invoke sorting function

	//Error handling
	if(stat ==0){
		printf("\n\r  Successfully invoked sort system call");

	}
	else{
		printf("sort System call not invoked properly");

		stat_var = -1;
	}

	//Print contents of sorted buffer
	printf("\n\rSorted buffer content is: ");
	for(i=0;i<buffer_size;i++)
		printf("\t%ld",*(sorted_buffer +i));

	//free the buffers
	free(src_buffer);
	free(sorted_buffer);

	if(stat_var == 0){
		printf("\n\rTEST 2 PASSED SUCCESSFULLY");
	}
	else{
		printf("\n\rTEST 2 FAILED");
	}
	
	/************************************************************************************************************************************************************
	 *
	 *								TEST CASE 3
	 *				DESCRIPTION:
	 *				PARAMETER 1: INPUT UNSORTED BUFFER IS ON HEAP AND CONTAINS INTEGER GENERATED BY rand() function
	 *				PARAMETER 2: OUTPUT SORTED BUFFER IS ON HEAP 
	 *				PARAMETER 3: SIZE IS LESS THAN 256
	 *				EXPECTED RESULT: FAIL SINCE SIZE IS LESS THAN 256
	 *
	 * **********************************************************************************************************************************************/
	//Sorting system call
	stat =-1;
	stat_var=0;
	buffer_size =5;
	if(buffer_size < 256){
		printf("\n\r Size is less than 256");
		stat_var= -1;
	}
	i=0;


	//Source buffer
	src_buffer = (int32_t *)malloc(sizeof(int32_t)*buffer_size);
	if(src_buffer == NULL){
		printf("\n\r Source buffer not allocated successfully");
		stat_var=-1;

	}

	//Sorted buffer

	sorted_buffer = (int32_t *)malloc(sizeof(int32_t)*buffer_size);
	if(sorted_buffer == NULL){
		printf("\n\rSorted buffer not allocated successfully");
		stat_var=-1;
	}
	//Print buffer size

	printf("\n\rBuffer size = %lu",(unsigned long)buffer_size);



	//Print contents of input buffer
	printf("\n\rInput buffer content is ");
	for(i=0;i<buffer_size;i++){
		*(src_buffer + i) = rand()%1024;		//Divide by 1024 so that everything will be within range of 0-1024
		printf("\t%ld",*(src_buffer +i));
	}


	stat =syscall(399,src_buffer,buffer_size,sorted_buffer);	//System call to invoke sorting function

	//Error handling
	if(stat ==0){
		printf("\n\r  Successfully invoked sort system call");


	}
	else{
		printf("sort System call not invoked properly");
		stat_var= -1;

	}

	//Print contents of sorted buffer
	printf("\n\rSorted buffer content is: ");
	for(i=0;i<buffer_size;i++)
		printf("\t%ld",*(sorted_buffer +i));

	free(sorted_buffer);
	free(src_buffer);
	if(stat_var == 0){
		printf("\n\rTEST 3 PASSED SUCCESSFULLY");
	}
	else{
		printf("\n\rTEST 3 FAILED");
	}


	/************************************************************************************************************************************************************
	 *
	 *								TEST CASE 4
	 *				DESCRIPTION:
	 *				PARAMETER 1: INPUT UNSORTED BUFFER CONTAINS NULL
	 *				PARAMETER 2: OUTPUT SORTED BUFFER IS ON HEAP 
	 *				PARAMETER 3: SIZE IS POSITIVE and GREATER THAN OR EQUAL TO 256
	 *				EXPECTED RESULT: FAIL SINCE UNSORTED BUFFER CONTAINS NULL
	 *
	 * **********************************************************************************************************************************************/
	//Sorting system call
	 stat =-1;
	 stat_var =0;
	buffer_size =256;
	if(buffer_size < 256){
		printf("\n\r Size is less than 256");
		stat_var= -1;
	}	 
	i=0;


	//Source buffer
	src_buffer = NULL;
	if(src_buffer == NULL){
		printf("\n\r Source buffer not allocated successfully");
		printf("\n\r Test case 3 failed");
		stat_var =-1;

	}

	//Sorted buffer

	sorted_buffer = (int32_t *)malloc(sizeof(int32_t)*buffer_size);
	if(sorted_buffer == NULL){
		printf("\n\rSorted buffer not allocated successfully");
		stat_var=-1;
	}
	//Print buffer size

	printf("\n\rBuffer size = %lu",(unsigned long)buffer_size);



	//Print contents of input buffer
	printf("\n\rInput buffer content is ");
	for(i=0;i<buffer_size;i++){
		*(src_buffer + i) = rand()%1024;		//Divide by 1024 so that everything will be within range of 0-1024
		printf("\t%ld",*(src_buffer +i));
	}


	stat =syscall(399,src_buffer,buffer_size,sorted_buffer);	//System call to invoke sorting function

	//Error handling
	if(stat ==0){
		printf("\n\r  Successfully invoked sort system call");


	}
	else{
		printf("sort System call not invoked properly");
		stat_var=-1;

	}

	//Print contents of sorted buffer
	printf("\n\rSorted buffer content is: ");
	for(i=0;i<buffer_size;i++)
		printf("\t%ld",*(sorted_buffer +i));

	free(sorted_buffer);
	if(stat_var == 0){
		printf("\n\rTEST 4 PASSED SUCCESSFULLY");
	}
	else{
		printf("\n\rTEST 4 FAILED");
	}
		

}
