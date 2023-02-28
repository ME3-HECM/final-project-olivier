#include <xc.h>
#include "dc_motor.h"
extern volatile char ForwardFlag = 1;
int _15dleftdelay = 100; //delay corresponging to a 15 degree turn left
int _15drightdelay = 100;//delay corresponding to a 15 degree turn right
int _1squaredelay = 1000; //delay corresponding to 1 square reversal
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

//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
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
    for (unsigned int i = 0; i <50; i++)//increase power value up to 50/100% of power (this value is tunable for how fast you want it to go)
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
void Left15(struct DC_motor *mL, struct DC_motor *mR)
{
    //this has been tuned to invoke a turn enough times for a 90 degree change
    turnLeft(mL,mR);//invoke the turn left
    __delay_ms(_15dleftdelay);
    stop(mL,mR);//stop the rotation of the buggy 
}

//function to turn right 90 degrees
void Right15(struct DC_motor *mL, struct DC_motor *mR)
{   
 //this has been tuned to invoke a turn enough times for a 90 degree change
    turnRight(mL,mR);//invoke the turn right
   __delay_ms(_15drightdelay);
    stop(mL,mR);//stop the rotation of the buggy 
}

void rotate180left(struct DC_motor *mL, struct DC_motor *mR)//from lab 6 
{
    turnLeft(mL,mR);//invoke the turn right
    __delay_ms(180);//tune this so it corresponds to a 180 degree change
    stop(mL,mR);//stop the rotation of the buggy 
}

void Red_R90(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){
        int i;
        for (i = 0; i = 5; i++){// execute the 15 degree turn 6 times
            Right15(mL,mR);
        }
    }
    else if(ForwardFlag == 0){
        //execute left 90 turn 
    }
}

void Green_L90(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){
        //execute left 90 turn
        int i;
        for (i = 0; i = 5; i++){// execute the 15 degree turn 6 times
            Left15(mL,mR);
        }
    }
    else if(ForwardFlag&0){
        //execute right 90 turn 
    }
}

void Blue_T180(struct DC_motor *mL, struct DC_motor *mR)
{
    //execute right 180 turn (depending on which side turns left/right more accurately)
    int i;
    for (i = 0; i = 11; i++){// execute the 15 degree turn 12 times to give a 180 degree change
        Left15(mL,mR);
    }
    
}
void Yellow_REV1_L90(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){
        //execute reverse 1 square & right 90 turn 
        fullSpeedReverse(mL,mR);
        __delay_ms(1squaredelay);//delay corresponding to 1 square reversed
        stop(mL,mR);//stop the buggy 
        Red_R90(mL,mR); //execute a 90 degree right turn
    }
    else if(ForwardFlag&0){
        //execute left turn 90 & forward 1 square 
        Green_L90(mL,mR); //execute a 90 degree right turn
        stop(mL,mR);//stop the buggy 
        fullSpeedAhead(mL,mR);
        __delay_ms(1squaredelay);//delay corresponding to 1 square reversed
        stop(mL,mR);
    }
}
void Pink_rev1_L90(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){
        //execute reverse 1 square & left 90 turn 
        fullSpeedReverse(mL,mR);
        __delay_ms(1squaredelay);//delay corresponding to 1 square reversed
        stop(mL,mR);//stop the buggy 
        Green_L90(mL,mR); //execute a 90 degree right turn
    }
    else if(ForwardFlag&0){
        //execute right turn 90 & forward 1 square 
        Red_R90(mL,mR); //execute a 90 degree right turn
        stop(mL,mR);//stop the buggy 
        fullSpeedAhead(mL,mR);
        __delay_ms(1squaredelay);//delay corresponding to 1 square reversed
        stop(mL,mR);
        
    }
}
void Orange_R135(struct DC_motor *mL, struct DC_motor *mR)
{
    if (ForwardFlag){
        //execute Right 135 turn
        int i;
        for (i = 0; i = 8; i++){// execute the 15 degree turn 9 times to give a 135 degree change
        Right15(mL,mR);
        }
        
    }
    else if(ForwardFlag&0){
        //execute Left 135 turn  
        int i;
        for (i = 0; i = 8; i++){// execute the 15 degree turn 9 times to give a 135 degree change
        Left15(mL,mR);
        }
    }
}
void LightBlue_L135(struct DC_motor *mL, struct DC_motor *mR)
{
     if (ForwardFlag){
        //execute Left 135 turn
        int i;
        for (i = 0; i = 8; i++){// execute the 15 degree turn 9 times to give a 135 degree change
        Left15(mL,mR);
        }
        
    }
    else if(ForwardFlag&0){
        //execute Right 135 turn  
        int i;
        for (i = 0; i = 8; i++){// execute the 15 degree turn 9 times to give a 135 degree change
        Right15(mL,mR);
        }
    }
}
void White(void)
{
    //calibration sequence for white 
    //here reverse the array containing the history of turn (i) and distance of the buggy travel (ii) 
}
