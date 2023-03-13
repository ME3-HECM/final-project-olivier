#include <xc.h>
#include "memory.h"
#include "colorfunctions.h"
#include "timer.h"
#include "main.h"
#include "dc_motor.h"

//the memory update function is called everytime a colour is reached

void memoryUpdate(struct RGBC_rel *cf, unsigned int movementCount, unsigned int *movementMemory, unsigned int *timerMemory)
{
    //get colour value and store it 
    int colourcode = cf->colourindex;
    //get the timer value and store it 
    unsigned int timerVal = getTimerValue(); //get the 16 bit time 
    timerMemory[movementCount] = timerVal;//store value of time taken for operation to occour in array
    movementMemory[movementCount] = colourcode;//store the colour in the movement array
}
void maxTimeReturn(void)
{
    memoryUpdate()
    White(&motorL,&motorR,movementCount,movementMemory,timerMemory);
}

