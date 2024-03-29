#include <xc.h>
#include "dc_motor.h"
#include "LED.h"
#include "memory.h"
#include "main.h"
#include "timer.h"


// function initialise T2 and CCP for DC motor control

void initDCmotorsPWM(unsigned int PWMperiod){
    //initialise your TRIS and LAT registers for PWM  
    TRISEbits.TRISE2=0; //output on RE2
    TRISCbits.TRISC7=0; //output on RC7
    TRISEbits.TRISE4=0; //output on RE2
    TRISGbits.TRISG6=0; //output on RC7
    
    LATEbits.LATE2=0; // 0 output on RE2
    LATCbits.LATC7=0; // 0 output on RC7
    LATEbits.LATE4=0; // 0 output on RE4
    LATGbits.LATG6=0; // 0 output on RG6
    
    //configure PPS to map CCP modules to pins
    RE2PPS=0x05; //CCP1 on RE2
    RE4PPS=0x06; //CCP2 on RE4
    RC7PPS=0x07; //CCP3 on RC7
    RG6PPS=0x08; //CCP4 on RG6

    // timer 2 config
    T2CONbits.CKPS=0b100; // 1:64 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=PWMperiod; //Period reg 10kHz base period
    T2CONbits.ON=1;
    
    //setup CCP modules to output PMW signals
    //initial duty cycles 
    CCPR1H=0; 
    CCPR2H=0; 
    CCPR3H=0; 
    CCPR4H=0; 
    
    //use tmr2 for all CCP modules used
    CCPTMRS0bits.C1TSEL=0;
    CCPTMRS0bits.C2TSEL=0;
    CCPTMRS0bits.C3TSEL=0;
    CCPTMRS0bits.C4TSEL=0;
    
    //configure each CCP
    CCP1CONbits.FMT=1; // left aligned duty cycle (we can just use high byte)
    CCP1CONbits.CCP1MODE=0b1100; //PWM mode  
    CCP1CONbits.EN=1; //turn on
    
    CCP2CONbits.FMT=1; // left aligned
    CCP2CONbits.CCP2MODE=0b1100; //PWM mode  
    CCP2CONbits.EN=1; //turn on
    
    CCP3CONbits.FMT=1; // left aligned
    CCP3CONbits.CCP3MODE=0b1100; //PWM mode  
    CCP3CONbits.EN=1; //turn on
    
    CCP4CONbits.FMT=1; // left aligned
    CCP4CONbits.CCP4MODE=0b1100; //PWM mode  
    CCP4CONbits.EN=1; //turn on
}

// function to set CCP PWM output from the values in the motor structure-----FROM LAB 6
void setMotorPWM(struct DC_motor *m)
{
    unsigned char posDuty, negDuty; //duty cycle values for different sides of the motor
    
    if(m->brakemode) {
        posDuty=(m->PWMperiod) - ((unsigned int)(m->power)*(m->PWMperiod))/100; //inverted PWM duty
        negDuty=(m->PWMperiod); //other side of motor is high all the time
    }
    else {
        posDuty=0; //other side of motor is low all the time
		negDuty=((unsigned int)(m->power)*(m->PWMperiod))/100; // PWM duty
    }
    
    if (m->direction) {
        *(m->posDutyHighByte)=posDuty;  //assign values to the CCP duty cycle registers
        *(m->negDutyHighByte)=negDuty;       
    } else {
        *(m->posDutyHighByte)=negDuty;  //do it the other way around to change direction
        *(m->negDutyHighByte)=posDuty;
    }
}

void RAMback(struct DC_motor *mL, struct DC_motor *mR)
{
    stop(mL,mR); //stop buggy 
    __delay_ms(100);
    fullSpeedReverse(mL,mR); // reverse 
    __delay_ms(1000); // until hit the wall with the colour
    stop(mL,mR);
}
//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    BrakeLightON;
    while((mL->power)>0 && (mR->power)>0)//whilst the power is non zero to each motor (in all functions power increases to both sides equally)
    {
        //individually check the power to each motor is above zero 
        if (mL->power>0){
            mL->power--;//decrement power by 1%      
        }
        if ((mR->power)>0){
            mR->power--; //decrement power by 1%
        }
        setMotorPWM(mL);//set new motor power values
        setMotorPWM(mR);
        __delay_us(400);//delay to decrease power slowly
    }
    BrakeLightOFF;
}

//function to make the robot turn left
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
    
    (mL->direction) = 0; //set motor direction backward for left
    (mR->direction) = 1; //set motor direction forward for right
    setMotorPWM(mL);//set new motor direction 
    setMotorPWM(mR);
    for (unsigned int i = 0; i <50; i++)//increase power value up to 50/100% of power (this value is tunable for how fast you want it to go)
    {
        mL->power++; //increase the power to both motors by 1%
        mR->power++;
        setMotorPWM(mL);//set new motor power values
        setMotorPWM(mR);
        __delay_us(100);//delay to increase power slowly
    } 
    
}

//function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR)
{ 
    (mL->direction) = 1; //set motor direction forward for left
    (mR->direction) = 0; //set motor direction backward for right
    setMotorPWM(mL);//set new motor direction 
    setMotorPWM(mR);
    for (unsigned int i = 0; i <50; i++)//increase power value up to 50/100% of power (this value is tunable for how fast you want it to go)
    {
        mL->power++; //increase the power to both motors by 1%
        mR->power++;
        setMotorPWM(mL);//set new motor power values
        setMotorPWM(mR);
        __delay_us(100);//delay to increase power slowly
    } 
}

//function to make the robot go straight
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR)
{
 (mL->direction) = 1; //set motor direction forward for left
 (mR->direction) = 1; //set motor direction forward for right 
 //here we will have a for loop to increase the power to the motor gradually
    for (unsigned int i = 0; i <30; i++)//increase power value up to 50/100% of power (this value is tunable for how fast you want it to go)
    {
        (mL->power)++; //increase the power to both motors by 1%
        (mR->power)++;
        setMotorPWM(mL);//set new motor power values
        setMotorPWM(mR);
        __delay_us(400);//delay to increase power slowly
    } 
}
void fullSpeedReverse(struct DC_motor *mL, struct DC_motor *mR)
{
 (mL->direction) = 0; //set motor direction forward for left
 (mR->direction) = 0; //set motor direction forward for right 
 //here we will have a for loop to increase the power to the motor gradually
    for (unsigned int i = 0; i <50; i++)//increase power value up to 50/100% of power (this value is tunable for how fast you want it to go)
    {
        (mL->power)++; //increase the power to both motors by 1%
        (mR->power)++;
        setMotorPWM(mL);//set new motor power values
        setMotorPWM(mR);
        __delay_us(400);//delay to increase power slowly
    } 
}

//function to turn left 90 degrees
void Left45(struct DC_motor *mL, struct DC_motor *mR)
{
    IndicatorLeftON;
    //this has been tuned to invoke a turn enough times for a 45 degree change
    stop(mL,mR); //stop buggy
    __delay_ms(50);
    turnLeft(mL,mR);//invoke the turn left
    __delay_ms(_45dleftdelay);
    stop(mL,mR);//stop the rotation of the buggy 
    __delay_ms(50);//minimise intertia from buggy between 45 degree changes 
    IndicatorLeftOFF;
}

//function to turn right 90 degrees
void Right45(struct DC_motor *mL, struct DC_motor *mR)
{   
    IndicatorRightON;
 //this has been tuned to invoke a turn enough times for a 45 degree change
    stop(mL,mR);
    __delay_ms(100);
    turnRight(mL,mR);//invoke the turn right
    __delay_ms(_45drightdelay);
    stop(mL,mR);//stop the rotation of the buggy 
    __delay_ms(100);//minimise intertia from buggy between 45 degree changes 
    IndicatorRightOFF;
    
}

void rotate180left(struct DC_motor *mL, struct DC_motor *mR)//from lab 6 
{
    //left turn 45x4 times
    Left45(mL,mR);
    Left45(mL,mR);
    Left45(mL,mR);
    Left45(mL,mR);
}

void reverseHalfSquare(struct DC_motor *mL, struct DC_motor *mR)
{
    stop(mL,mR); //stop buggy (it hits the wall)
    __delay_ms(100);
    fullSpeedReverse(mL,mR); // reverse 
    __delay_ms(_halfsquare); // until a half square is reached
    stop(mL,mR);
    __delay_ms(100);
}

void Red_R90(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){ 
        reverseHalfSquare(mL,mR);//reverse half square
        Right45(mL,mR);
        Right45(mL,mR);
    }
    else {
        //execute left 90 turn 
        Left45(mL,mR);
        Left45(mL,mR);
    }
}

void Green_L90(struct DC_motor *mL, struct DC_motor *mR)
{   
    if (ForwardFlag){
        reverseHalfSquare(mL,mR);//reverse half square
        //execute left 90 turn
        Left45(mL,mR);
        Left45(mL,mR);
    }
    else {
        //execute right 90 turn 
        Right45(mL,mR);
        Right45(mL,mR);
    }
}

void Blue_T180(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){
        reverseHalfSquare(mL,mR);//reverse half square
        //execute right 180 turn (depending on which side turns left/right more accurately)
        rotate180left(mL,mR); 
    }else{
        rotate180left(mL,mR);
    }
    
}
void Yellow_rev1_R90(struct DC_motor *mL, struct DC_motor *mR)
{ 
    if (ForwardFlag){
        reverseHalfSquare(mL,mR);//reverse half square
        //execute reverse 1 square & right 90 turn 
        fullSpeedReverse(mL,mR);
        __delay_ms(_1square);//delay corresponding to 1 square reversed
        //execute a 90 degree right turn
        Right45(mL,mR);
        Right45(mL,mR);
    }
    else{
        //execute right turn 90 & forward 1 square 
        //execute a 90 degree right turn
        Left45(mL,mR);
        Left45(mL,mR); //execute a 90 degree left turn
    }
}
void Pink_rev1_L90(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){
        reverseHalfSquare(mL,mR);//reverse half square
        //execute reverse 1 square & left 90 turn 
        fullSpeedReverse(mL,mR);
        __delay_ms(_1square);//delay corresponding to 1 square reversed
        Left45(mL,mR);
        Left45(mL,mR); //execute a 90 degree left turn
    }
    else {
        //execute a 90 degree right turn
        Right45(mL,mR);
        Right45(mL,mR);      
    }
}
void Orange_R135(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){
        reverseHalfSquare(mL,mR);//reverse half square
        //execute Right 135 turn
        Right45(mL,mR);
        Right45(mL,mR);
        Right45(mL,mR);
    }
    else{
        //execute Left 135 turn  
        Left45(mL,mR);
        Left45(mL,mR);
        Left45(mL,mR);
    }
}
void LightBlue_L135(struct DC_motor *mL, struct DC_motor *mR)
{
     
     if (ForwardFlag){
        reverseHalfSquare(mL,mR);//reverse half square
        //execute Left 135 turn  
        Left45(mL,mR);
        Left45(mL,mR);
        Left45(mL,mR);
       
    }
    else{
         //execute Right 135 turn
        Right45(mL,mR);
        Right45(mL,mR);
        Right45(mL,mR);     
    }
}
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
                RAMback(mL,mR);}
            else if (movementMemory[i]==4){
                Pink_rev1_L90(mL,mR);
                RAMback(mL,mR);}
            else if (movementMemory[i]==5){
                Orange_R135(mL,mR);}
            else if (movementMemory[i]==6){
                LightBlue_L135(mL,mR);}
            else if (movementMemory[i]==7){
                stop(mL,mR);}//7 = white so just stop and then carry on
            RAMback(mL,mR);
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
