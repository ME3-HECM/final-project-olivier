#include <xc.h>
#include "main.h"

extern volatile unsigned int maxTime;
void Interrupts_init(void)
{
    //RC4 interrupt Enable bit for serial port and TX4 FOR TRANSMITTER   
    PIE4bits.RC4IE=1; //the E at the end is for enable
    
    INTCONbits.IPEN=1;//Enable priority level setting
    //This sets the priority to high  interrupt  bit
    //IPR4bits.RC4IP=1;// The P at the end is for priority and
   // IPR4bits.TX4IP=1;
    
    INTCONbits.PEIE=1;//ENABLE PERIPHERAL INTERRUPTS
    //Interrupt control INTCON register, enables all interrupts globally
    INTCONbits.GIE=1;
    // It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.
}

/************************************
* High priority interrupt service routine
* Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
// When reg is cleared this flag is set and the ISR 
{  
    if (PIR0bits.TMR0IF) {//interrupt source is the timer overflowing (16.77 seconds)
    maxTime = 1;//
    PIR0bits.TMR0IF=0;//turn off interrupt flag 
    }
}
