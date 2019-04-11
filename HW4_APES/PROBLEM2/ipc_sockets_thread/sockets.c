/*********************************************************************************************
 *                      FILENAME: sockets.c
 * *****************************************************************************************/

/*
 * @Title: Bi directional communication between two threads using sockets
 * @Author: Vikrant Waje
 * @Date: 19  February 2019
 * @Tools: buildroot, vim,gcc
 */

/*********************************************************************************
 *                        HEADER FILE SECTION
 * **********************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<signal.h>


/*********************************************************************************
 *                        GLOBAL VARIABLE SECTION
 * **********************************************************************************/

//LED command
typedef enum LED{
	ON,OFF,TOGGLE,FAULT,WARNING, CRITICAL, SAFE, BLINK, DAMAGED, OUT_OF_RANGE
}led_cmd_t;	

//thread1 send packet
typedef struct thread1_message{
	char messages[40];
	led_cmd_t command;
	int string_len;

}thread1_cmd_t;

//thread2 send packet
typedef struct thread2_message{
	char messages[40];
	led_cmd_t command;
	int string_len;
}thread2_cmd_t;

//timestamp
double time_child =0;
double time_master =0;


struct timespec timestamp;
struct timespec master_timestamp;

//signal structure
struct sigaction user_defined;
int network_socket,server_socket;
int flag;

thread1_cmd_t *ptr = NULL;
thread2_cmd_t *ptr1 = NULL;

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
	t1[6].command= DAMAGED;
	t1[7].command= OUT_OF_RANGE;

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
	
	//get socket connection
	server_socket = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in server_address ;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	//bind it to a address
	bind(server_socket,(struct sockaddr *)&server_address, sizeof(server_address));
	listen(server_socket,5);
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL); 

	thread1_cmd_t receive_buff;
	FILE *fptr = fopen(arg,"a+");

	fprintf(fptr,"\n%c%c%lf%c%s %s %lu %s %ld",'\n','[',time_child,']'," Entering thread1,","pid = ",pthread_self(),"Resource identifier: ",server_socket);
	fclose(fptr);
	while(1){
		if(flag ==1){//if signal is received
		fptr=fopen(arg,"a+");
		clock_gettime(CLOCK_MONOTONIC,&timestamp);
		time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
		fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_child,']'," Exiting thread1 due to kill signal,","pid = ",pthread_self());
		fclose(fptr);
	
		pthread_exit(NULL);
		}
		
		if(i ==10){	//send ten messages

			clock_gettime(CLOCK_MONOTONIC,&timestamp);
			time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
			fptr=fopen(arg,"a+");
			fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_child,']'," Exiting thread1,","pid = ",pthread_self());
			fclose(fptr);
			pthread_exit(NULL);

		}
		//		printf("\n\rt1.message");
		send(client_socket,&t1[i],sizeof(thread1_cmd_t),0);	//send messages


		//			printf("\n\rt1.read_message");

		recv(client_socket,&receive_buff,sizeof(thread1_cmd_t),0);	//read messages
		i++;



		fptr = fopen(arg,"a+");
		clock_gettime(CLOCK_MONOTONIC,&timestamp);
		time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
		fprintf(fptr,"\n%c%c%lf%c%s  %s %d  %s %d ",'\n','[',time_child,']', receive_buff.messages, "String length:",receive_buff.string_len,", LED command for which aknowledgement was received from thread2: ",receive_buff.command);
		fclose(fptr);

		//	sleep(1);


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
	

	//thread2 messages
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




	thread2_cmd_t server_response;
	network_socket = socket(AF_INET, SOCK_STREAM,0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	//connect to socket
	status = connect(network_socket,(struct sockaddr *)&server_address, sizeof(server_address)); 

	FILE *fptr = fopen(arg,"a+");

	fprintf(fptr,"\n%c%c%lf%c%s %s %lu %s %ld",'\n','[',time_child,']'," Entering thread2,","pid = ",pthread_self(),"Resource identifier: ",network_socket);
	fclose(fptr);
	while(1){
		if(flag ==1){//if signal is received
		fptr=fopen(arg,"a+");
		clock_gettime(CLOCK_MONOTONIC,&timestamp);
		time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
		fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_child,']'," Exiting thread2 due to kill signal,","pid = ",pthread_self());
		fclose(fptr);
	
		pthread_exit(NULL);
		}

		if(i ==10){	//send ten messages
			clock_gettime(CLOCK_MONOTONIC,&timestamp);
			time_child = ((timestamp.tv_sec *1000.0)+ (timestamp.tv_nsec/1000000.0));	
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
		recv(network_socket,&server_response,sizeof(thread2_cmd_t),0);	//read messages

		fprintf(fptr,"\n%c%c%lf%c%s  %s %d  %s %d ",'\n','[',time_child,']', server_response.messages,"String length: ",server_response.string_len,",LED command sent by thread1: ",server_response.command);
		
	//	fprintf(fptr,"%c%c%lf%c%s %d  ",'\n','[',time_child,']', server_response.messages,server_response.command);
	
	//	fprintf(fptr,"%c%c%lf%c%d ",'\n','[',time_child,']', receive_buff.command);

		/*	fprintf(fptr,"%s ",receive_buff.messages);
			fprintf(fptr," %d",receive_buff.command);*/
		fclose(fptr);
		sleep(1);
		//		printf("\n\rt2.send_message");

		send(network_socket,&t2[i],sizeof(thread2_cmd_t),0);	//send messages

		sleep(1);
		i++;
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

	//signal structure
	user_defined.sa_flags=0;
	user_defined.sa_handler = kill_thread_handler;	//kill thread handler
	sigemptyset(&user_defined.sa_mask);
	sigaction(SIGINT,&user_defined,NULL);	//Associate SIGINT with kill thread handler


	FILE *fptr = fopen(argv[1],"a+");
	clock_gettime(CLOCK_MONOTONIC,&master_timestamp);
	time_master = ((master_timestamp.tv_sec *1000.0)+ (master_timestamp.tv_nsec/1000000.0));	
	fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_master,']'," Main thread:Using SOCKETS for INTER PROCESS COMMUNICATION","pid =" ,pthread_self());//log into the file
	fclose(fptr);
	int status = 0;




	pthread_t thread1,thread2;
	status = pthread_create(&thread1,NULL,thread1_handler,argv[1]);	//Create thread1
	if(status !=0){
		perror("\n\rThread 1 creation failed: ");
	}
	sleep(3);
	status =pthread_create(&thread2,NULL,thread2_handler,argv[1]);	//Create thread2
	if(status !=0){
		perror("\n\rThread 2 creation failed: ");
	}

	status = pthread_join(thread1,0);		//Wait for thread1 to join
	if(status !=0){
		perror("\n\rThread 1 join unsuccessfull");
	}
	status = pthread_join(thread2,0);		//Wait for thread2 to join
	if(status !=0){
		perror("\n\rThread 2 join unsuccessfull");
	}

	clock_gettime(CLOCK_MONOTONIC,&master_timestamp);
	time_master = ((master_timestamp.tv_sec *1000.0)+ (master_timestamp.tv_nsec/1000000.0));	
	fptr=fopen(argv[1],"a+");
	fprintf(fptr,"\n%c%c%lf%c%s %s %lu",'\n','[',time_master,']'," EXiting main thread,","pid = ",pthread_self());
	fclose(fptr);
	
	close(server_socket);
	close(network_socket);

}
