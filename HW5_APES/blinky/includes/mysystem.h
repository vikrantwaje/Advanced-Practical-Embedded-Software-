/***********************************************************************************
 * @file myerror.h
 * @brief Contains commonly implemented error routine
 * @author Vikrant Waje
 * @date April 7, 2018
 *
 *****************************************************************************/

#ifndef INCLUDES_MYSYSTEM_H_
#define INCLUDES_MYSYSTEM_H_

//***********************************************************************************
//                              Include files
//***********************************************************************************
#include "mygpio.h"
#include "myclock.h"
#include "myuart.h"
#include "myi2c.h"
#include "myled.h"
#include "mytimer.h"


//***********************************************************************************
//                                  Macros
//***********************************************************************************


//***********************************************************************************
//                              Global variables
//***********************************************************************************


//***********************************************************************************
//                              Function Prototype
//***********************************************************************************
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*
  @brief: Initialise all the peripherals required for system operation.


 @param: None
 @param:None

 @return: None
 */
/*-----------------------------------------------------------------------------------------------------------------------------*/
void system_init();


#endif /* INCLUDES_MYSYSTEM_H_ */