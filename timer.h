#ifndef _timer_H
#define _timer_H

#include <xc.h>
/********************************************//**
 *  Function to initialise Timer0 module
 ***********************************************/
void Timer0_init(void);
/********************************************//**
 *  Function to reset Timer0 eachtime a card is recognised
 ***********************************************/
void TimerReset(void);
/********************************************//**
 *  Function to retrieve the timer0 value and 
 *convert to milliseconds for the memory functions
 ***********************************************/
float getTimerValue(void);
#endif 
