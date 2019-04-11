#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

void main(){
	uint8_t i =0;
	uint8_t j=0;
	int status = 0;
	char names_t[5][10] ={"Vikrant","Raj","Nikhil","Akash","Rick"};
	char temp[10];

	for(i=0;i<5;i++){
		printf("\n\r%s",names_t[i]);

	}

	printf("\n-----------------------\n\r---------------------------");

	for(i=0;i<5;i++){
		for(j=i+1;j<5;j++){
			status = strcmp(names_t[i],names_t[j]);
			if(status >0){
				strcpy(temp,names_t[i]);
				strcpy(names_t[i],names_t[j]);
				strcpy(names_t[j],temp);
			}
		}	

	}

	for(i=0;i<5;i++){
		printf("\n\r%s",names_t[i]);

	}
}
