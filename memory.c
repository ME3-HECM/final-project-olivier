#include <xc.h>
#include "memory.h"
#include "colorfunctions.h"
#include "timer.h"
#include "main.h"
#include "dc_motor.h"

//the memory update function is called everytime a colour is reached

void memoryUpdateMovement(struct RGBC_rel *cf, volatile unsigned int movementCount, volatile unsigned int *movementMemory)
{
    //get colour value and store it 
    int colourcode = cf->colourindex;
    movementMemory[movementCount] = colourcode;//store the colour in the movement array
}
void memoryUpdateTime(volatile unsigned int movementCount, volatile unsigned int *timerMemory)//updates the corresponding memory time
{
    unsigned int timerVal = getTimerValue()-_halfsquare-650;//get the 16 bit time and minus the half square delay plus the time it takes to recognise its a wall
    timerMemory[movementCount] = timerVal;//store value of time taken for operation to occour in array
}
void maxTimeReturn(void)
{
    White(&motorL,&motorR,movementCount,movementMemory,timerMemory);//perform the return home function 
    maxTime = 0;//reset the max time flag 
    //perform the white function here to return home
}

