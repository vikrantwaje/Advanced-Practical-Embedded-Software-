/*********************************************************************************************
 *			FILENAME: cron.c
 * *****************************************************************************************/

/*
 * @Title: Scheduling a bunch of system call every ten minutes using crontab
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
 * @brief: four system calls are invoked
 * ------->getpid(): To get the process id
 *  ------>getuid(): To get user id
 *  ------>time() and ctime(): To get current date and time
 *  ------>syscall(399): User defined function for sorting numbers in descending order
 */

void main(){

	FILE *fptr;	//file pointer
	fptr=fopen("cron.txt","a+");	//Open the file in append mode
	if(fptr == NULL){		//Error checking
		printf("\n\rError in opening file");
	}



	//Current Process ID 
	int pid_no = getpid();

	//Current user ID
	int uid_no = getuid();


	//Current date and time
	time_t t;
	t = time(&t);
	t = ctime(&t);

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



	for(i=0;i<buffer_size;i++){
		*(src_buffer + i) = rand()%255;
		
	}


	stat =syscall(399,src_buffer,buffer_size,sorted_buffer);

	if(stat ==0){
		printf("\n\r  Successfully invoked sort system call");

	}
	else{
		printf("sort System call not invoked properly");

	}

	
	fprintf(fptr,"\n\r***********************************************************************************************************");
	fprintf(fptr,"\n\rGetpid=%d \n\ruserid=%d\n\rCurrent date and time=%s",pid_no,uid_no,t);	//Log everything into cron.txt

	fprintf(fptr,"\n\rBuffer size = %lu",(unsigned long)buffer_size);				
	fprintf(fptr,"\n\n\rInput buffer content is: ");
	
	for(i=0;i<buffer_size;i++){
		if(i%8==0){
			fprintf(fptr,"\n\r");
		}	
		fprintf(fptr,"\t%ld",*(src_buffer +i));
	}

	//Sorted buffer
	fprintf(fptr,"\n\n\rSorted buffer content is ");
	for(i=0;i<buffer_size;i++){
		if(i%8==0){
			fprintf(fptr,"\n\r");
		}	
		fprintf(fptr,"\t%ld",*(sorted_buffer +i));
	}

	fclose(fptr);
	free(src_buffer);
	free(sorted_buffer);
}
