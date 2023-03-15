# Course project - Mine navigation search and rescue

## Challenge brief

Our task was to develop an autonomous robot that can find it's way through a "mine" using a sequence of coloured cards corresponding to specific movements and it must also be able to return to its starting position from any point on it's journey.  From the brief we have broken down our tasks as such: 

- [x]  Navigate towards a coloured card and stop before impacting the card
- [x]  Read the card colour
- [x]  Interpret the card colour using a predefined code and perform the navigation instruction
- [x]  When the final card is reached, navigate back to the starting position
- [x]  Handle exceptions and return back to the starting position if final card cannot be found

## "Mine" environment specification

A "mine" is contstructed from black plywood walls 100mm high with some walls having coloured cards located on the sides of the maze to assist with navigation. The following colour code is to be used for navigation:

Colour | Instruction
---------|---------
Red | Turn Right 90
Green | Turn Left 90
Blue | Turn 180
Yellow | Reverse 1 square and turn right 90
Pink | Reverse 1 square and turn left 90
Orange | Turn Right 135
Light blue | Turn Left 135 
White | Finish (return home)
Black | Maze wall colour

### Using the buggy

1. Place the buggy at the entrance to the mine in the center of the first square.
1. Line the buggy up with the intial direction of travel.
1. Turn the PIC board on.
1. When ready to start, turn the main buggy power on switch

### Buggy Light Sensor Cover

The colour sensor is covered in a 3D printed shroud to stop ambient light interfering with the sensor. (courtesy of Martin England)


## Solution Management and design
In order to give structure to our working, we produced a Gantt chart of tasks we felt needed to be done in design, implementation and testing phases with deadlines to keep progress on track. The tasks were also delegated so we could work in parallel to each other to increase efficiency. As with any project, we ran into hurdles and update the Gantt chart accordingly to add or remove tasks as neccesity prompted

![Gantt Chart](https://github.com/ME3-HECM/final-project-olivier/blob/main/Screenshot%202023-03-15%20125240.jpg)

## Solution Evolution
### Colour Calibration
Initially we planned to use raw RGB values from the converter but realised these fluctate a lot with light so instead moved to RGB values relative to total light however it was rather difficult to detect colours with relative values so we decided on Hue Values.Serial communication was used to read out the RGBC values from the sensors, the calculated hues then finally when calibrated, the perceived colour based on the hue of the surface being faced.

```
char* Hue2Colour(struct RGBC_rel *cf)
{
    char* colourname = "";
  
    if (((cf->h)>352)&&((cf->h)<360))
    {
        //colourname for serial printing
        colourname = "Red";
        //colourindex can be picked up by other functions
        //to process relevant action
        cf->colourindex=0;
    }
     if (((cf->h)>75)&&((cf->h)<79))
    {
        colourname = "Green";
        cf->colourindex=7; //gg
    }
    else if (((cf->h)>26)&&((cf->h)<29))
    {
        colourname = "Eggshell";
        cf->colourindex=1;
    }
    else if (((cf->h)>15)&&((cf->h)<19))
    {
        colourname = "Pink";
        cf->colourindex=2;
    }
    else if (((cf->h)>21)&&((cf->h)<25))
    {
        colourname = "Yellow";
        cf->colourindex=3;
    }
    else if (((cf->h)>7)&&((cf->h)<9))
    {
        colourname = "Orange";
        cf->colourindex=4;
    }
    else if (((cf->h)>85)&&((cf->h)<94))
    {
        colourname = "Light Blue";
        cf->colourindex=5;
    }
    else if (((cf->h)>145)&&((cf->h)<164))
    {
        colourname = "Dark Blue";
        cf->colourindex=6;
    }
    
    return colourname;
}
```

To maintain consistency we used a black cover over the colourclick when calibrating the hue ranges for each colour card, this was so our calibration was independent of ambient light kevels. This way we did not have to change the hue ranges when the buggy was deployed during the challenge.

### Returning Home
As per the challenge brief, under two circumstances the buggy would be reuqired to retrace it's steps back to it's starting point. This is when it sees the white card or if it has spent too much time looking for said card without success. After a move is executed, a number corresponding to this move is saved in an array. When it is time to go home, this array is then read from the end to the start executing the opposite of each of the moves to retrace it's steps.
To be able to record the distance the buggy went forward and replay it on the way back Timer0 were used.
```
//the memory update function is called everytime a colour is reached

void memoryUpdateMovement(struct RGBC_rel *cf, volatile unsigned int movementCount, volatile unsigned int *movementMemory)
{
    //get colour value and store it 
    int colourcode = cf->colourindex;
    movementMemory[movementCount] = colourcode;//store the colour in the movement array
}
void memoryUpdateTime(volatile unsigned int movementCount, volatile float *timerMemory)//updates the corresponding memory time
{//get the 16 bit time and minus the half square delay plus the approximate time it takes to recognise the colour
    float timerVal = getTimerValue()-_halfsquare-_recogniseColour;
    timerMemory[movementCount] = timerVal;//store value of time taken for operation to occour in array
}
void maxTimeReturn(void)
{
    stop(&motorL,&motorR);
    maxTime = 0;
    //perform the white function here to return home
}

```

```
/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source

    T0CON1bits.T0CKPS=0b1101; // 1:8192
    T0CON0bits.T016BIT=1;	//16bit mode	
	PIE0bits.TMR0IE = 1; // Enable TMR0 interrupt
    
    // it's a good idea to initialise the timer registers so we know we are at 0
    TMR0H=0;            //write High reg first, update happens when low reg is written to
    TMR0L=0;           // start timer at 0 in binary (16 bit, high register is the first 8 bits and the low is the second 8 bits)	

    T0CON0bits.T0EN=1;	//start the timer
    
}
void TimerReset(void)
{
    TMR0L = 0;//reset timer
    TMR0H = 0;
}
float getTimerValue(void)
{
    int timerCount; // Declare unsigned integer variable
    // Read low byte of TMR0 and store in lower 8 bits of timerCount (integers store 16 bits)
    timerCount = TMR0L;
    // Read high byte of TMR0, shift left 8 bits, and combine with low byte of TMR0
    timerCount |= (int)(TMR0H << 8);
    return timerCount*1.048576; // Return 16-bit timer value in ms 
}
 
```
The interrupt was used to record the amount of time between each move i.e the time spent travelling between each card and make the buggy travel for the same amount or time on the way back. Below is the memory update function using interrupts.
 ```
void memoryUpdateMovement(struct RGBC_rel *cf, volatile unsigned int movementCount, volatile unsigned int *movementMemory)
{
    //get colour value and store it 
    int colourcode = cf->colourindex;
    movementMemory[movementCount] = colourcode;//store the colour in the movement array
}
void memoryUpdateTime(volatile unsigned int movementCount, volatile float *timerMemory)//updates the corresponding memory time
{//get the 16 bit time and minus the half square delay plus the approximate time it takes to recognise the colour
    float timerVal = getTimerValue()-_halfsquare-_recogniseColour;
    timerMemory[movementCount] = timerVal;//store value of time taken for operation to occour in array
}
```
Interrupt overflow was also used to recognise when too much time had elapsed without finding a card and would cause the buggy to trigger the return home sequence.
```
insert code here
```



