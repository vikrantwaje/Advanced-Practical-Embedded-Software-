/*
 * @Title: Perform various File IO operation
 * @Author: Vikrant Waje
 * @Date: 26th January 2019
 * @Tools used: Ubuntu, Git, vim
 * @References: 
 * */
/***************************************************
 *		Header files
 * ************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

void main(){
	
	uint8_t * allocated_array = NULL;	//Dynamically allocated array
	uint8_t status = 0; //variable for recording status
	uint8_t c = 0; //Variable to store character
	printf("\n\r This is file containing program for file IO operation using C programming");	//Printing some string on standard IO

	FILE* file1 = NULL; //Set file pointer to NULL
        const char *file_name ="myfileIO.txt";	//Name of file
	//const char * file_mode = "r+";	//Mode of file: Read write
	/*file1=fopen(file_name,file_mode); //Create a file named myfile.txt and set mode as read write*/
	file1 = fopen(file_name,"w"); //Create file

	if(file1 == NULL){	//Check for error
	printf("\n\rfile  failed to open");
	}

	file1 = fopen(file_name,"r+"); //Set permisions as read-write
	if(file1 == NULL){	//Check for error
	printf("\n\rfile failed to open");
	}

	file1 =fopen(file_name,"w+"); //Open file for writing
	if(file1 == NULL){	//Check for error
	printf("\n\rfile failed to open");
	}

	fputc('v',file1); //Write a single character into file
	
	status = fclose(file1);	//Close the file
	if(status!=0){	//Check the status
		printf("\n\rError in closing the file");
	}

	file1 = fopen(file_name,"a");	//Open file in append mode
	if(file1 == NULL){	//Check for error
	printf("\n\rfile failed to open in append mode");
	}

	allocated_array = malloc(20 * sizeof(uint8_t));	//Dynamically allocate array of 20 bytes
	if(allocated_array == NULL){			//Check error
	printf("\n\rArray failed to be allocated 20 bytes");
	}
	
	printf("\n\rEnter the string that is to inserted into myfileIO.txt: ");
	scanf("%[^\n]s",allocated_array);	//Read from command line

	//fputs(allocated_array,file1);	//Insert the string into myfileIO.txt
	//while(*(allocated_array + i)!='\0'){
	fprintf(file1,"%s",(allocated_array ));
	//i++;
	//}

	status = fflush(file1);	//Flush any buffered output
	if(status!=0){	//Checks the status
	printf("\n\rError in fflush ");
	}

	status = fclose(file1); //close the file
	if(status!=0){
		printf("\n\rError in closing the file");
	}

	file1 = fopen(file_name,"r");	//Open file for reading
	if(file1 == NULL){	//Check for status
		printf("\n\rError in opening the file");
	}

	c = fgetc(file1);	//Read character from file
	if(feof(file1))
		printf("\n\rEnd of file indicator");
	else
		printf("\n\r%c",c);	//print character on command line
	
	if (fgets(allocated_array,(strlen(allocated_array) +1) ,file1)!=NULL){	//print the string in file on command line
		printf("%s",allocated_array);
	}

	status = fclose(file1);	//Close the file
	if(status!=0){
		printf("\n\rError in closing the file");
	}
	free(allocated_array);	//free the allocated memory
	allocated_array = NULL ; //Prevention of dangling pointer
}


