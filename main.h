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

volatile unsigned int maxTime = 0;

void main(void);

#endif /* MAIN_H */
