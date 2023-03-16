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
2. Line the buggy up with the intial direction of travel.
3. Turn the PIC board on and the buggy.
4. When ready to start, press the RF2 button and step away.

[Buggy in Use](https://drive.google.com/file/d/1ZWeQf-ztbf95jfq3j8RBCIMzxyjYJchO/view?usp=sharing)

## Solution Management and design
In order to give structure to our working, we produced a Gantt chart of tasks we felt needed to be done in design, implementation and testing phases with deadlines to keep progress on track. The tasks were also delegated so we could work in parallel to each other to increase efficiency. As with any project, we ran into hurdles and update the Gantt chart accordingly to add or remove tasks as neccesity prompted

![Gantt Chart](https://github.com/ME3-HECM/final-project-olivier/blob/main/Screenshot%202023-03-15%20125240.jpg)

Working in parallel we used different branches to design the movement system, the colour sensing process and another branch solely for calibrating the colours and to debug.
## Solution Evolution
### Colour Calibration
Initially we planned to use raw RGB values from the converter but realised these fluctate a lot with light so instead moved to RGB values relative to total light however it was rather difficult to detect colours with just relative values so we decided on Hue Values in combination to differentiate between similar colours of overlapping hues.Serial communication was used to read out the RGBC values from the sensors, the calculated hues then finally when calibrated, the perceived colour based on the hue of the surface being faced.

```
char* Hue2Colour(struct RGBC *cf, struct RGB_rel *rel)
{
    char* colourname = "";
    if ((((cf->h)>0)&&((cf->h)<3)|((cf->h)>352)&&((cf->h)<=360)))
    {
        //colourname for serial printing
        colourname = "Red";
        //colourindex can be picked up by other functions
        //to process relevant action
        cf->colourindex=0;
    }
    else if (((cf->h)>14)&&((cf->h)<18))
    {
        colourname = "Pink";
        cf->colourindex=4;
    }
    else if (((cf->h)>85)&&((cf->h)<130))
        {colourname = "Dark Blue";
        cf->colourindex=2;}
    else if (((cf->h)>6)&&((cf->h)<12))
    {
        colourname = "Orange";
        cf->colourindex=5;
    }
     else if (((cf->h)>65)&&((cf->h)<80))
    {
         //Green and blue values coincide but easy to differentiate when combined
         //with relative values so use that
        if (rel->B > 0.22)
        {     colourname = "Light Blue";
            cf->colourindex=6;}
        else {
            colourname = "Green";
            cf->colourindex=1; 
        }
    }
    else if (((cf->h)>18)&&((cf->h)<27))
    {
        if (rel->R > 0.51)
        {       
                  colourname = "Yellow";
            cf->colourindex=3;
            }
        else {
            colourname = "White";
            cf->colourindex=7; 
        } 
    }
    //If it hits a black wall or cannot recognise the colour it goes home
    else{cf->colourindex=8;}
    return colourname;
}
```
### Buggy Cover

To maintain consistency we used a black cover over the colourclick when calibrating the hue ranges for each colour card, this meant our calibration was independent of ambient light levels. This way we did not have to change the hue ranges when the buggy was deployed during the challenge. Also it meant there was no calibration needed before each maze. (Thank you Martin England for the cover design)

![Black Cover](https://github.com/ME3-HECM/final-project-olivier/blob/main/IMG_4356.jpg)

### Colour2Action
Finally, once a card has been recognised it's specific action is called by referring to the number associated to it.
Colour | Number
------------- | -------------
Red  | 0
Green  | 1 
Blue  | 2
Yellow  | 3 
Pink  | 4
Orange  | 5
Light Blue  | 6
White  | 7
Black/Unknown  | 8
```
void Colour2Action(struct RGBC *cf)
{
    if (cf->colourindex == 0)
    {
        Red_R90(&motorL,&motorR);
    }
    else if (cf->colourindex == 1)
    {
        Green_L90(&motorL,&motorR);
    }
    else if (cf->colourindex == 2)
    {
        Blue_T180(&motorL,&motorR);
    }
    else if (cf->colourindex == 3)
    {
        Yellow_rev1_R90(&motorL,&motorR);
    }
    else if (cf->colourindex == 4)
    {
        Pink_rev1_L90(&motorL,&motorR);
    }
    else if (cf->colourindex == 5)
    {
        Orange_R135(&motorL,&motorR);
    }
    else if (cf->colourindex == 6)
    {
        LightBlue_L135(&motorL,&motorR);
    }
    else if (cf->colourindex == 7)
    {
        White(&motorL,&motorR,movementCount,movementMemory,timerMemory);

    }//otherwise unknown colour most likely a black wall (Exception handling)
    else {White(&motorL,&motorR,movementCount,movementMemory,timerMemory);}

}
```

### Returning Home
As per the challenge brief, under two circumstances the buggy would be required to retrace it's steps back to it's starting point. This is when it sees the white card or if it has spent too much time looking for said card without success. After a move is executed, a number corresponding to this move is saved in an array. When it is time to go home i.e. it has read the white card, this array is then read from the end to the start executing the opposite of each of the moves to retrace it's steps.
To be able to record the distance the buggy moved between cards and replay it on the way back Timer0 was used.
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
### White Function
In the White function we included a ram backwards function for the retracing, this is to align the buggy with the wall and negate the issue of battery charge affecting the drift of the buggy over the course of a full maze.
```
void White(struct DC_motor *mL, struct DC_motor *mR,unsigned int movementCount, volatile unsigned int *movementMemory, volatile float *timerMemory)
{
    BrakeLightON;
    
    while (!retracingDone){
        reverseHalfSquare(mL,mR);//reverse half square
        rotate180left(mL,mR); //rotate buggy 180 degrees to face the reverse direction of the maze
        __delay_ms(500);//delay starting the retrace
        ForwardFlag = 0;//now put it in reverse mode 
        retracingDone = 1;//say the retracing is now done and so it will exit after the for loop
        //execute the reverse of all the commands sent 
        for (int i=movementCount; i>=0;i--){
            BrakeLightON;
            __delay_ms(200); //ensure the momentum of the buggy has been dissipated
            if (movementMemory[i]==0){
                Red_R90(mL,mR);}
            else if (movementMemory[i]==1){
                Green_L90(mL,mR);}
            else if (movementMemory[i]==2){
                Blue_T180(mL,mR);}
            else if (movementMemory[i]==3){
                Yellow_rev1_R90(mL,mR);
                RAMback(mL,mR);}//double ram to account for the 1 square forwards
            else if (movementMemory[i]==4){
                Pink_rev1_L90(mL,mR);
                RAMback(mL,mR);}//double ram to account for the 1 square forwards
            else if (movementMemory[i]==5){
                Orange_R135(mL,mR);}
            else if (movementMemory[i]==6){
                LightBlue_L135(mL,mR);}
            else if (movementMemory[i]==7){
                stop(mL,mR);}//7 = white so just stop and then carry on
            RAMback(mL,mR);//align with the wall
            BrakeLightON; 
            //now we read the timer memory to find the time between functions 
            float tempTimer = 0;
            TimerReset();//reset the timer in order to count up from 0->timerMemory[i]
            fullSpeedAhead(mL,mR);
            if (timerMemory[i]<0)//checking if the recognise colour delay makes it negative
            {
                while(tempTimer <(timerMemory[i]+_recogniseColour))
                {
                    tempTimer = getTimerValue();
                }
            }else if (timerMemory[i]>0){
                while(tempTimer<timerMemory[i])
                {
                    tempTimer = getTimerValue();
                }
            }
            stop(mL,mR);//stop the buggy and perform the action 
        }
    }
    stop(mL,mR);// stop buggy after retracing is done 
    __delay_ms(1000);
    BrakeLightOFF;
}
```
### Timer0
Below is the code for Timer0
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
### Memory function
The Timer was used to record the amount of time between each move i.e the time spent travelling between each card and make the buggy travel for the same amount of time on the way back. Below is the memory update function. 
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
### Exceptions
When the buggy has spent too long looking for a card it will also be required to retrace it's steps back, this was implemented using interrupts as seen below. The maxTime flag is picked up from the main and triggers the go home routine to retrace it's steps as it assumes itself to be lost. The overflow time implemented with the interrupt function was 33.55 seconds.
```
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
    maxTime = 1;//trigger the max time 
    PIR0bits.TMR0IF=0;//turn off interrupt flag 
    }
}
```

## Main Code
```
        while (!retracingDone){ //run this code until the white function is called (retracingDone is set to 1 in the White)
        fullSpeedAhead(&motorL,&motorR);//move the buggy forwards
        //wait to run into a wall
        while (!wall){
            colour_read_all(&colorf,&colorrel);//read RGB values from colour clicker
             //when in contact with a wall or card a lot more light is reflected
             //by sensors so the clear value falls
            if (colorf.Cf>300)//wait for the clear value to be over a certain threshold since a card will reflect more light
	    {
                memoryUpdateTime(movementCount,timerMemory);//update the time taken for action to occur corresponding to the movement
                //flag that a wall has been detected
                wall=1;
                __delay_ms(200);//this delay makes sure that the colour is constant when being read
                stop(&motorL,&motorR);//stop the buggy
            //If too much time has elapsed between cards then return home
            }
            else if (maxTime==1)
            {
                memoryUpdateTime(movementCount,timerMemory);
                colorf.colourindex=7;
                //flag that a wall has been detected
                wall=1;
                stop(&motorL,&motorR);//stop the buggy
            }
        }
        //Normal routine when buggy is not lost
        if (maxTime!=1){
            colour_read_all(&colorf,&colorrel);//read the colours from the colour click
            wall=0;//reset wall variable to reenter inner while loop after
            RGB2Hue(&colorf);//takes the RGB values and outputs hue 
            Hue2Colour(&colorf,&colorrel);//takes the hue and outputs the colour
        }
        memoryUpdateMovement(&colorf,movementCount,movementMemory);//update the memory function
        Colour2Action(&colorf);//perform the action
        if (colorf.colourindex == 7)//if the white function is called
        {
            while(!retracingDone){}//wait until the retracing is done before resetting the timer as it might mess up the white function
        }
        TimerReset();//reset the timer in order to have time between actions
        movementCount++; //increment the movement count
    }
```


