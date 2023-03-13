#include <xc.h>
#include "timer.h"

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b1100; // 1:32768
    T0CON0bits.T016BIT=1;	//16bit mode	
	
    // it's a good idea to initialise the timer registers so we know we are at 0
    TMR0H=0b00000000;            //write High reg first, update happens when low reg is written to
    TMR0L=0b00000000;           // start timer at 0 in binary (16 bit, high register is the first 8 bits and the low is the second 8 bits)
    T0CON0bits.T0EN=1;	//start the timer
}
void TimerReset(void)
{
    TMR0L = 0;//reset timer
    TMR0H = 0;
}
unsigned int getTimerValue(void)
{
    unsigned int timerCount; // Declare unsigned integer variable
    // Read low byte of TMR0 and store in lower 8 bits of timerCount
    timerCount = TMR0L;
    // Read high byte of TMR0, shift left 8 bits, and combine with low byte of TMR0
    timerCount |= (unsigned int)(TMR0H << 8);
    return timerCount; // Return 16-bit timer value
}
 