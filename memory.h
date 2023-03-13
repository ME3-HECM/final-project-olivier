#ifndef _memory_H
#define	_memory_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "memory.h"
#include "colorfunctions.h"
#include "timer.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz

unsigned int movementMemory[20];
unsigned int timerMemory[20];
void memoryUpdate(struct RGBC_rel *cf, unsigned int movementCount, unsigned int *movementMemory, unsigned int *timerMemory);
void maxTimeReturn(void);

#endif