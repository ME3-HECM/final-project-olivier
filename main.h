#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include "serial.h"
#include <stdio.h>
#include "colorclick.h"
#include "i2c.h"
#include "colorfunctions.h"
#include "dc_motor.h"
#include "LED.h"

#define _XTAL_FREQ 64000000

volatile unsigned int maxTime = 0;//define max time between counters for an overflow 
volatile unsigned int movementCount = 0;//define the movement counter
volatile unsigned int movementMemory[20] = {};//corresponds to R90,L90,T180
volatile float timerMemory[20] = {};//this corresponds to 1 second passing
void main(void);

#endif /* MAIN_H */
