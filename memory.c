#include <xc.h>
#include "memory.h"
#include "colorfunctions.h"
#include "timer.h"
#include "main.h"
#include "dc_motor.h"

//the memory update function is called everytime a colour is reached
void memoryUpdateMovement(struct RGBC *cf, volatile unsigned int movementCount, volatile unsigned int *movementMemory)
{
    //get colour value and store it 
    int colourcode = cf->colourindex;
    movementMemory[movementCount] = colourcode;//store the colour in the movement array
}
void memoryUpdateTime(volatile unsigned int movementCount, volatile float *timerMemory)//updates the corresponding memory time
{//get the 16 bit time and minus the half square delay plus the approximate time it takes to recognise the colour
    float timerVal = getTimerValue()-_recogniseColour;
    timerMemory[movementCount] = timerVal;//store value of time taken for operation to occour in array
}


