/*********************************************************************************************
 *                      FILENAME: shared memory.c
 * *****************************************************************************************/

/*
 * @Title: Bi directional communication between two threads using shared memory
 * @Author: Vikrant Waje
 * @Date: 19  February 2019
 * @Tools: buildroot, vim,gcc
 */

/*********************************************************************************
 *                        HEADER FILE SECTION
 * **********************************************************************************/
#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<signal.h>


/*********************************************************************************
 *                        GLOBAL VARIABLE SECTION
 * **********************************************************************************/

//LED commands
typedef enum LED{
	ON,OFF,TOGGLE,FAULT,WARNING,CRITICAL,SAFE,BLINK,DAMAGED,OUT_OF_RANGE
}led_cmd_t;	

//Thread1 packet
typedef struct thread1_message{
	char messages[40];
	led_cmd_t command;
	int string_len;

}thread1_cmd_t;

//Thread2 packet
typedef struct thread2_message{
	char messages[40];
	led_cmd_t command;
	int string_len;

}thread2_cmd_t;

//Timestamp variables
double time_child =0;
double time_master =0;

//flags
int send = 0;
int flag =0;

//Timestamp variables
struct timespec timestamp;
struct timespec master_timestamp;
//shared memory descriptor
int shd_des1, shd_des2;

//signal structure
struct sigaction user_defined;
/*******************************************************************************
 * @Name: Kill thread callback function
 * @brief: Responsible for killing threads when CTRL+C  is triggered
 * @param: num: value of signal
 * @return: NONE
 ******************************************************************************/
void kill_thread_handler(int num){
	if(num == SIGINT){
		printf("\n\rUSR2 triggered");
		flag =1;
	}
}

thread1_cmd_t *ptr = NULL;
thread2_cmd_t *ptr1 = NULL;

/*******************************************************************************
 * @Name:  thread1 callback function
 * @brief: Communicate with thread2
 * @param: arg: log file
 * @return: NONE
 ******************************************************************************/
void *thread1_handler(void *arg){
	int i =0;
	int status =0;
	int num =0;

	clock_gettime(CLOCK_MONOTONIC,&timestamp);
	time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));

	thread1_cmd_t t1[10];
	//thread1 messages
	strcpy(t1[0].messages,"Thread1: Initialising communication");
	strcpy(t1[1].messages,"Thread1: Opening gateway");
	strcpy(t1[2].messages,"Thread1: Checking system status");
	strcpy(t1[3].messages,"Thread1: Adding parity bits");
	strcpy(t1[4].messages,"Thread1: Sending data");
	strcpy(t1[5].messages,"Thread1: Extracting data");
	strcpy(t1[6].messages,"Thread1: Extracting parity");
	strcpy(t1[7].messages,"Thread1: Waiting for Acknowledgement");
	strcpy(t1[8].messages,"Thread1: Closing the gateway");
	strcpy(t1[9].messages,"Thread1: Closing the connection");
	//thread1 command
	t1[0].command = ON;
	t1[1].command =OFF;
	t1[2].command= TOGGLE;
	t1[3].command= FAULT;
	t1[4].command = WARNING;
	t1[5].command = CRITICAL;
	t1[6].command= SAFE;
	t1[7].command= BLINK;	
	t1[8].command= DAMAGED;
	t1[9].command= OUT_OF_RANGE;
	//thread1 string length
	t1[0].string_len = strlen(t1[0].messages);
	t1[1].string_len = strlen(t1[1].messages);
	t1[2].string_len = strlen(t1[2].messages);
	t1[3].string_len = strlen(t1[3].messages);
	t1[4].string_len = strlen(t1[4].messages);
	t1[5].string_len = strlen(t1[5].messages);
	t1[6].string_len = strlen(t1[6].messages);
	t1[7].string_len = strlen(t1[7].messages);
	t1[8].string_len = strlen(t1[8].messages);
	t1[9].string_len = strlen(t1[9].messages);
	


	thread1_cmd_t receive_buff;	//Receive buffer
	FILE *fptr = fopen(arg,"a+");	//open file

	fprintf(fptr,"\n%c%c%lf%c%s %s %lu %s %p",'\n','[',time_child,']'," Entering thread1,","pid = ",pthread_self(),"Resource identifier: ",ptr);
	fclose(fptr);
	while(1){
		if(flag ==1){ //if signal received, exit gracefully
		fptr=fopen(arg,"a+");
		clock_gettime(CLOCK_MONOTONIC,&timestamp);
		time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
		fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_child,']'," Exiting thread1 due to kill signal,","pid = ",pthread_self());
		fclose(fptr);
	
		pthread_exit(NULL);
		}
		if(i ==10){	//Send strings from thread1 to thread2

			clock_gettime(CLOCK_MONOTONIC,&timestamp); //timestamp
			time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
			fptr=fopen(arg,"a+");
			fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_child,']'," Exiting thread1,","pid = ",pthread_self());
			fclose(fptr);   //close file handler
			pthread_exit(NULL);

		}
		//		printf("\n\rt1.message");
		strcpy(ptr->messages, t1[i].messages);
		ptr->command = t1[i].command;
		ptr->string_len = t1[i].string_len;
		
		sleep(1);
		//			printf("\n\rt1.read_message");

		while(send == 0){	//Wait for send flag from thread2
		if(flag ==1){
		fptr=fopen(arg,"a+");
		clock_gettime(CLOCK_MONOTONIC,&timestamp);
		time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
		fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_child,']'," Exiting thread1 due to kill signal,","pid = ",pthread_self());
		fclose(fptr);
	
		pthread_exit(NULL);
		}

	}
		fptr = fopen(arg,"a+");
		clock_gettime(CLOCK_MONOTONIC,&timestamp);
		time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
		fprintf(fptr,"\n%c%c%lf%c%s  %s %d  %s %d ",'\n','[',time_child,']', ptr1->messages, "String length:",ptr1->string_len,", LED command for which acknowledgement was received from thread2: ",ptr1->command);

		//	fprintf(fptr,"%c%c%lf%c%d ",'\n','[',time_child,']', receive_buff.command);
		/*fprintf(fptr,"%s",receive_buff.messages);
		  fprintf(fptr," %d",receive_buff.command);*/
		fclose(fptr);
		send =0;
		i++;

	//		sleep(1);


	}
}


/*******************************************************************************
 * @Name:  thread2 callback function
 * @brief: Communicate with thread1
 * @param: arg: log file
 * @return: NONE
 ******************************************************************************/
void * thread2_handler(void *arg){
	int status=0;
	int i =0;	
	int num =0;
	clock_gettime(CLOCK_MONOTONIC,&timestamp);
	time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
	thread2_cmd_t t2[10];
	//thread2 message
	strcpy(t2[0].messages,"Thread2: Communication initialised");
	strcpy(t2[1].messages,"Thread2: Gateway opened");
	strcpy(t2[2].messages,"Thread2: Status positive");
	strcpy(t2[3].messages,"Thread2: Parity bits added successfull");
	strcpy(t2[4].messages,"Thread2: Data sent");
	strcpy(t2[5].messages,"Thread2: Data extracted");
	strcpy(t2[6].messages,"Thread2: Parity extracted");
	strcpy(t2[7].messages,"Thread2: Acknowledgement sent");
	strcpy(t2[8].messages,"Thread2: Gateway closed");
	strcpy(t2[9].messages,"Thread2: Connection closed");

	//thread2 command
	t2[0].command = ON;
	t2[1].command =OFF;
	t2[2].command= TOGGLE;
	t2[3].command= FAULT;
	t2[4].command = WARNING;
	t2[5].command = CRITICAL;
	t2[6].command= SAFE;
	t2[7].command= BLINK;	
	t2[8].command= DAMAGED;
	t2[9].command= OUT_OF_RANGE;

	//thread2 string length
	t2[0].string_len = strlen(t2[0].messages);
	t2[1].string_len = strlen(t2[1].messages);
	t2[2].string_len = strlen(t2[2].messages);
	t2[3].string_len = strlen(t2[3].messages);
	t2[4].string_len = strlen(t2[4].messages);
	t2[5].string_len = strlen(t2[5].messages);
	t2[6].string_len = strlen(t2[6].messages);
	t2[7].string_len = strlen(t2[7].messages);
	t2[8].string_len = strlen(t2[8].messages);
	t2[9].string_len = strlen(t2[9].messages);



	FILE *fptr = fopen(arg,"a+");

	fprintf(fptr,"\n%c%c%lf%c%s %s %lu %s %p",'\n','[',time_child,']'," Entering thread2,","pid = ",pthread_self(),"Resource identifier: ",ptr1);	//log
	fclose(fptr);
	while(1){
		if(flag ==1){	//flag set when signal triggered
		fptr=fopen(arg,"a+");
		clock_gettime(CLOCK_MONOTONIC,&timestamp);
		time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
		fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_child,']'," Exiting thread2 due to kill signal,","pid = ",pthread_self());
		fclose(fptr);
		pthread_exit(NULL);
		}
		if(i ==10){	//send ten messages from thread2 to thread1
		
			clock_gettime(CLOCK_MONOTONIC,&timestamp);
			time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
			//break;
			fptr=fopen(arg,"a+");	
			fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_child,']'," Exiting thread2,","pid = ",pthread_self());

			fclose(fptr);
			
			pthread_exit(NULL);

		}

		//		printf(" %d",receive_buff.command);
		//		printf("\n\rt2.read_message");

		fptr =fopen(arg,"a+");
		clock_gettime(CLOCK_MONOTONIC,&timestamp);
		time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
		//sleep(1);	
	fprintf(fptr,"\n%c%c%lf%c%s  %s %d  %s %d ",'\n','[',time_child,']', ptr->messages, "String length:",ptr->string_len,", LED command sent by thread1: ",ptr->command);


		//	fprintf(fptr,"%c%c%lf%c%d ",'\n','[',time_child,']', receive_buff.command);

		/*	fprintf(fptr,"%s ",receive_buff.messages);
			fprintf(fptr," %d",receive_buff.command);*/
		fclose(fptr);
		
		//		printf("\n\rt2.send_message");

		//send = 1;
		//write messages to shared area
		strcpy(ptr1->messages,t2[i].messages);
		ptr1->command = t2[i].command;
		ptr1->string_len = t2[i].string_len;
		i++;
		send =1;	//set the send flag
		sleep(1);
		
	}

}


/*******************************************************************************
 * @Name:  Main function
 * @brief: Wait until communication finishes
 * @param: argc: Count of argument
	   argv: Value of argument
 * @return: NONE
 ******************************************************************************/
void main(int argc, char **argv){

	//signal structuree
	user_defined.sa_flags=0;
	user_defined.sa_handler = kill_thread_handler;	//kill thread handler
	sigemptyset(&user_defined.sa_mask);	//dont mask
	sigaction(SIGINT,&user_defined,NULL);	//associate kill thread handler to SIGINT


	FILE *fptr = fopen(argv[1],"a+");
	clock_gettime(CLOCK_MONOTONIC,&master_timestamp);	//timestamp
	time_master = ((master_timestamp.tv_sec *1000.0)+ (master_timestamp.tv_nsec/1000000.0));	
	fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_master,']'," Main thread:Using SHARED MEMORY for INTER PROCESS COMMUNICATION","pid =" ,pthread_self());
	fclose(fptr);
	int status = 0;

	shd_des1 = shm_open("/shared_mem1",O_RDWR | O_CREAT,0666);	//shared memory descriptor
	if(shd_des1 == -1){
		perror("\n\rFailed to open shared memory 1");
	}
	ftruncate(shd_des1,sizeof(thread1_cmd_t));	//allocate shared memory of particular size
	ptr = mmap(0,sizeof(thread1_cmd_t), PROT_WRITE|PROT_READ,MAP_SHARED,shd_des1,0);
	if(ptr == (void *)-1){
		perror("memory mapping not done properly");
	}

	shd_des2 = shm_open("/shared_mem2",O_RDWR | O_CREAT,0666);	//open shared memory
	if(shd_des2 == -1){
		perror("\n\rFailed to open shared memory 2");
	}
	ftruncate(shd_des2,sizeof(thread2_cmd_t));

	ptr1=mmap(0,sizeof(thread2_cmd_t), PROT_WRITE | PROT_READ, MAP_SHARED, shd_des2,0);
	if(ptr1 == (void *)-1){
		perror("memory mapping not done properly");
	}

	pthread_t thread1,thread2;
	status = pthread_create(&thread1,NULL,thread1_handler,argv[1]);	//create thread1
	if(status !=0){
		perror("\n\rThread 1 creation failed: ");
	}
		sleep(3);
	status =pthread_create(&thread2,NULL,thread2_handler,argv[1]);	//create thread2
	if(status !=0){
		perror("\n\rThread 2 creation failed: ");
	}

	status = pthread_join(thread1,0);	//wait for thread1 to join
	if(status !=0){
		perror("\n\rThread 1 join unsuccessfull");
	}
	status = pthread_join(thread2,0);	//wait for thread2 to join
	if(status !=0){
		perror("\n\rThread 2 join unsuccessfull");
	}

	clock_gettime(CLOCK_MONOTONIC,&master_timestamp);
	time_master = ((master_timestamp.tv_sec *1000.0)+ (master_timestamp.tv_nsec/1000000.0));	
	fptr=fopen(argv[1],"a+");
	fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_master,']'," EXiting main thread,","pid = ",pthread_self());
	fclose(fptr);
	status = shm_unlink("/shared_mem1");	//unlink shared memory 1
	if(status == -1){
		perror("\n\rFailed to unlink the shared memory");
	}
	status = shm_unlink("/shared_mem2");	//unlink shared memory 2
	if(status == -1){
		perror("\n\rFailed to unlink the shared memory");
	}
}
