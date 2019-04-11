/***********************************************************************************************************
 *
 *				DOCUMENTATION SECTION
 *
 * **************************************************************************************************************/
/**********************************************************************************************************
 * @Title: Memory copy function
 * @Author: Vikrant Waje
 * @Date: 12 February 2019
 * @Tools used: vim,arm-linux-gcc. beaglebone green
 *

/**************************************************************************************************************
 *
 *				HEADER FILES
 *
 * ****************************************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>



/***********************************************************************************************************
 *@Name: memcpy functiom
 *@param: src: Pointer to source
 *@param:dst: pointer to destination
 *@return: None
 *******************************************************************************************************/

void memcpy(void *src, void *dst,size_t length){
	size_t i =0;
	uint8_t *src_cpy = (uint8_t *)src;
	uint8_t *dst_cpy = (uint8_t *)dst;

	for(i=0;i<length;i++){
	
	*(dst_cpy + i) = *(src_cpy + i);
	
	}



}




void main(){
	uint8_t i =0;
	uint8_t *src = malloc(sizeof(uint8_t)*10);
	uint8_t *dst = malloc(sizeof(uint8_t)*10);
	for(i=0;i<10;i++)
		*(src +i) =i;
	memcpy(src,dst,10);	
	for(i=0;i<10;i++)
		printf("\n\r%d",*(dst +i));


}
