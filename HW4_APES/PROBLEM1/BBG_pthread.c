/*********************************************************************************************
 *                      FILENAME: BBG_pthread.c
 * *****************************************************************************************/

/*
 * @Title: Simple pthread application that spawns three threads
    Thread1: Master thread, waits for completing the execution of both child threads
    Thread2: Child thread 1, responsible for counting the characters in the file
    Thread3: Child thread2, responsible for posting the CPU utilization every 100 msec into the log file
 * @Author: Vikrant Waje
 * @Date: 19  February 2019
 * @Tools: buildroot, vim,gcc
 */

/*********************************************************************************
 *                        HEADER FILE SECTION
 * **********************************************************************************/
#include<pthread.h>
#include<stdio.h>
#include<sys/types.h>
#include<time.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>

/*********************************************************************************
 *                        GLOBAL VARIABLE SECTION
 * **********************************************************************************/
int pid_master;	//store pid of master process
int pid_child1;	//store id of child thread
int pid_child2; //store id of child thread

// Structure for timestamp
struct timespec master_timestamp;
struct timespec child1_timestamp;
struct timespec child2_timestamp;

//Structure for timer
struct sigevent signal_specification;
timer_t timer_child2;
struct itimerspec new_setting;

//structure for USR1
struct sigaction act;
struct sigaction user_defined;

int flag=0;	//flag to indicate interrupt has occured
int flag2=0;	//flag to indicate USR2 has been triggered
void timer_handler(int num){
	//	if(count%9 ==0)
	flag =1;
	//	else 
	//		flag =0;

}
/*******************************************************************************
 * @Name: Kill thread callback function
 * @brief: Responsible for killing threads when USR2 is triggered
 * @param: num: value of signal
 * @return: NONE
 ******************************************************************************/

void kill_thread_handler(int num){
	printf("\nUSR2 triggered");
	flag2=1;
}

/*******************************************************************************
 * @Name: child thread1 callback function
 * @brief: Responsible for processing the text file and count the characters
 * @param: child_thread_param: Pointer to file pointer which is the log file
 * @return: NONE
 ******************************************************************************/

void *child1_thread_callback(void *child1_thread_param){

	double time_child1 =0;	//store timestamp value
	clock_gettime(CLOCK_MONOTONIC,&child1_timestamp);	//get time
	time_child1 = ((child1_timestamp.tv_sec *1000.0)+ (child1_timestamp.tv_nsec/1000000.0));	//convert it into millisecond format
	char *file_name = (char *)child1_thread_param;	//typecast void * parameter into file pointer
	pid_child1 = pthread_self();	//get the thread id
	FILE *fptr = NULL;		//Initialise pointer to NULL
	fptr = fopen(file_name,"a+");	//Open file
	if(fptr == NULL){		//Check for NULL
		printf("\n\rFile %s failed to open by child1 thread",file_name);
	}
	fprintf(fptr,"%c%c%lf%c%s %s %u",'\n','[',time_child1,']'," Entering Child1,","pid = ",pid_child1);	//log into the file
	printf("\nHello from child1 thread ");	//print on console
	 
	char *str= NULL ;	//pointer to text file given by user for character processing
	size_t i=0;
	int pos=0;
	FILE *fptr2=fopen("gdb.txt","rb");	//open the text file
	if(fptr2==NULL){			//Check for NULL value
	printf("\nCant open file");
	}
	fseek(fptr2,0,SEEK_END);		//move fptr2 to end of file
	size_t s = ftell(fptr2); 		//get the offset from start
	rewind(fptr2);				//move back the fptr2 to original starting position
	str = malloc(s);			//allocate memory corresponding to length of file
	if(str == NULL)				//check for NULL
	printf("\n\rBuffer not allocated");
	if(str !=NULL){
	fread(str,s,1,fptr2);			//Read from file
	}

	int count_array[26] = {0};		//keep an array for storing letters A-Z

	for(i=0;i<s;i++){			
		if(flag2 == 1){			//if interrupt by signal, exit
			clock_gettime(CLOCK_MONOTONIC,&child1_timestamp);
	time_child1 = ((child1_timestamp.tv_sec *1000.0)+ (child1_timestamp.tv_nsec/1000000.0));
	
	fprintf(fptr,"%c%c%lf%c%s %s %u",'\n','[',time_child1,']',"Exiting Child1 thread without completing work,","pid = ",pid_child1);	//timestamp value
	fclose(fptr);
	fclose(fptr2);
	pthread_exit(NULL);
			
		}
	else{
		if(*(str +i)>=97  && *(str + i)<=122){	//check for ascii value of character and increment the array 
			pos =(*(str +i)) -'a';	
			count_array[pos]++;		
		}
		else if(*(str +i)>=65 && *(str +i)<=90){	//check for uppercase character
			pos=(*(str +i)) +32 - 'a';
			count_array[pos]++;

		}
		else{
		
		}
	}
}
	//printf("%lu",s);
	fprintf(fptr,"%s","\t Processed data: ");	
	for(i=0;i<26;i++){		
	if(flag2 == 1){			//if signal received, exit
			clock_gettime(CLOCK_MONOTONIC,&child1_timestamp);
	time_child1 = ((child1_timestamp.tv_sec *1000.0)+ (child1_timestamp.tv_nsec/1000000.0));
	
	fprintf(fptr,"%c%c%lf%c%s %s %u",'\n','[',time_child1,']',"Exiting Child1 thread without completing work,","pid = ",pid_child1);
	fclose(fptr);
	fclose(fptr2);
	pthread_exit(NULL);
	}			//else print character only if it has occured less than 100 times
	else{
		if(count_array[i]<=100)
	
		fprintf(fptr,"%c:%d\t",'a'+i,count_array[i] );
}
//	fclose(fptr2);
	

}
free(str);	//free the storage for text file
fclose(fptr2);
	
	clock_gettime(CLOCK_MONOTONIC,&child1_timestamp);
	time_child1 = ((child1_timestamp.tv_sec *1000.0)+ (child1_timestamp.tv_nsec/1000000.0));
	
	fprintf(fptr,"%c%c%lf%c%s %s %u",'\n','[',time_child1,']',"Exiting Child1 thread after completing work,","pid = ",pid_child1);
	fclose(fptr);
	pthread_exit(NULL);	//exit the thread

}

/*******************************************************************************
 * @Name: child thread2 callback function
 * @brief: Responsible for posting CPU utilization every 100 msec
 * @param: child_thread_param: Pointer to file pointer which is the log file
 * @return: NONE
 ******************************************************************************/

void *child2_thread_callback(void *child2_thread_param){
	int i=0;
	long long unsigned int cpu_time,idle_time,cpu_usage;	//state variables to keep track of CPU usage
	double cpu_utilization;	//final CPU utilization after calculation
	int cpu[9];;		//store the cpu parameter associated for calculating cpu utilization
	char *substr;
	char *input_string = malloc(sizeof(char) *100);	//stores the string from proc/stat
	FILE *fptr1 = NULL;

	double time_child2 =0;	//timestamp value
	clock_gettime(CLOCK_MONOTONIC,&child2_timestamp);	//get the time
	time_child2 = ((child2_timestamp.tv_sec *1000.0)+ (child2_timestamp.tv_nsec/1000000.0));	//convert it into milliseconds
	char *file_name = (char *)child2_thread_param;
	pid_child2 = pthread_self();	//get the thread id
	FILE *fptr = NULL;
	fptr = fopen(file_name,"a+");	//open the file
	if(fptr == NULL){		//check for NULL condition
		printf("\n\rFile %s failed to open by child2 thread",file_name);

	}

	fprintf(fptr,"%c%c%lf%c%s %s %u",'\n','[',time_child2,']'," Entering Child2,","pid = ",pid_child2);	//log into the file
	printf("\nHello from child2 thread ");	//print into the console
	while(1){
		if(flag2==1){		//if signal USR2 has been received

			clock_gettime(CLOCK_MONOTONIC,&child2_timestamp);	//timestamp
			time_child2 = ((child2_timestamp.tv_sec *1000.0)+ (child2_timestamp.tv_nsec/1000000.0));	//convert it into millisecond
			fprintf(fptr,"%c%c%lf%c%s %s %u",'\n','[',time_child2,']'," Signal USR2 received, Exiting Child2 thread,","pid = ",pid_child2);	//log into the file
			flag2=0;
			fclose(fptr);
			pthread_exit(NULL);
		}
		else  if(flag == 1){	//if timer has expired it's period of 100 msec
			fptr1 = fopen("/proc/stat","r");
			fgets(input_string,47,fptr1);	//get the string from proc/stat
			//			puts(input_string);
			substr = strtok(input_string," ");	//get string until next space character
			for(i=0;i<9;i++){
				substr = strtok(NULL," ");	//extract every string between two spaces
				cpu[i] = atoi(substr);		//convert the string into integers to be used for calculation

			}
			cpu_time = cpu[0]+cpu[1]+cpu[2]+cpu[3]+cpu[4]+cpu[5]+cpu[6]+cpu[7];	//total cpu time
			idle_time = cpu[3] +cpu[4];	//idle time calculation
			cpu_usage = cpu_time - idle_time;	//cpu usage
			cpu_utilization = (float)cpu_usage/cpu_time  ;	//cpu utilization calculation
			clock_gettime(CLOCK_MONOTONIC,&child2_timestamp);
			time_child2 = ((child2_timestamp.tv_sec *1000.0)+ (child2_timestamp.tv_nsec/1000000.0));

			fprintf(fptr,"%c%c%lf%c %s%lf",'\n','[',time_child2,']',"CPU Utlization= ",cpu_utilization);	//log into the file
			flag =0;
		}
	}

}

/*******************************************************************************
 * @Name: Main function/thread
 * @brief: Waits for both child thread
 * @param: argc: Count of arguments passed from command line
	   argv: Value of paramter passed
 * @return: NONE
 ******************************************************************************/
void main(int argc, char **argv){
	act.sa_flags=0;	//
	act.sa_handler = timer_handler;	//timer handler
	sigemptyset(&act.sa_mask);	//dont mask any signal
	sigaction(SIGUSR1,&act,NULL);	//associate SIGUR1 with timer handler

	user_defined.sa_flags=0;
	user_defined.sa_handler = kill_thread_handler;	//kill thread handler
	sigemptyset(&user_defined.sa_mask);	//Dont mask any signal
//	sigaddset(&user_defined.sa_mask,SIGUSR1);
	sigaction(SIGUSR2,&user_defined,NULL);	//Associate SIGUSR2 with kill thread handler

//	pthread_sigmask(SIG_BLOCK,&user_defined,NULL);

	signal_specification.sigev_signo = SIGUSR1;
	//Specifying the time interval and time period for timer
	signal_specification.sigev_value.sival_int = 15;
	timer_create(CLOCK_REALTIME,&signal_specification,&timer_child2);
	new_setting.it_value.tv_sec = 1;
	new_setting.it_value.tv_nsec =0;
	new_setting.it_interval.tv_sec =0;
	new_setting.it_interval.tv_nsec =100000000;
	timer_settime(timer_child2,0,&new_setting,NULL);


	pthread_t  child1,child2;
	double time_master =0;
	clock_gettime(CLOCK_MONOTONIC,&master_timestamp);	//timestamp
	time_master =((master_timestamp.tv_sec*1000.0)+ (master_timestamp.tv_nsec/1000000.0));	//print into log file
	char *file_name = (char *)argv[1];
	int pid_master = pthread_self();	//get threead id
	FILE *fptr = NULL;
	fptr = fopen(file_name,"a+");
	if(fptr == NULL){
		printf("\n\rFile %s failed to open by master thread",file_name);
	}
	fprintf(fptr,"%c%c%lf%c%s %s %u",'\n','[',time_master,']',"Entering Master thread,","pid = ",pid_master);
	fclose(fptr);
	printf("\nHello from master thread ");
	int status =0;

	status = pthread_create(&child1,NULL,child1_thread_callback,argv[1]);	//Create thread1
	if(status !=0){
		printf("\n\rChild1 thread not created successfully");

	}
	status = pthread_create(&child2,NULL,child2_thread_callback,argv[1]);	//create thread2
	if(status !=0){
		printf("\n\rChild2 thread not created successfully");

	}
//	pthread_kill(child1,SIGUSR2);
	status=pthread_join(child1,NULL);		//Wait for child1 thread to finish
	if(status !=0){
	printf("\n\rJoining of child thread 1 not successfull");
	}
	status=pthread_join(child2,NULL);		//Wait for child2 thread to finish
	if(status !=0){
	printf("\n\rJoining of child thread 2 not successfull");
	}
	//pthread_exit(NULL);

	fptr=fopen(file_name,"a+");
	clock_gettime(CLOCK_MONOTONIC,&master_timestamp);

	time_master =((master_timestamp.tv_sec*1000.0)+ (master_timestamp.tv_nsec/1000000.0));	
	fprintf(fptr,"%c%c%lf%c%s %s %u",'\n','[',time_master,']',"Exiting Master thread,","pid = ",pid_master);	//log to file while exiting main thread
	fclose(fptr);	//close file handler

}
