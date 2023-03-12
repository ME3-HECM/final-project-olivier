#include <xc.h>
#include "LED.h"

void LED_init(){
    //set state of pins to output
    TRISDbits.TRISD4 = 0; // Brake light
    TRISHbits.TRISH0 = 0; // Right indicators
    TRISFbits.TRISF0 = 0; // Left indicator
    
    //set all LED's off 
    LATDbits.LATD4 = 0;// Brake light
    LATHbits.LATH0 = 0;// Right indicators
    LATFbits.LATF0 = 0;// Left indicators
}


