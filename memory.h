#ifndef _memory_H
#define	_memory_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "memory.h"
#include "colorfunctions.h"
#include "timer.h"
#include "main.h"
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz
/********************************************//**
 *  Function to update coded movements buggy makes
 ***********************************************/
void memoryUpdateMovement(struct RGBC *cf, volatile unsigned int movementCount, volatile unsigned int *movementMemory);
/********************************************//**
 *  Function to update the time memory array with time inbetween recognising cards
 ***********************************************/
void memoryUpdateTime(volatile unsigned int movementCount, volatile float *timerMemory);


#endif