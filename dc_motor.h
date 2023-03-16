#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>
#include "dc_motor.h"
#include "LED.h"
#include "memory.h"
#define _XTAL_FREQ 64000000

//variable for a forward motion
volatile char ForwardFlag = 1;
//variable for if the white function is done 
volatile unsigned int retracingDone = 0;
unsigned int _45dleftdelay = 144; //delay corresponging to a 45 degree turn left
unsigned int _45drightdelay = 144; //delay corresponding to a 45 degree turn right
unsigned int _1square = 800; //delay corresponding to 1 square travel
unsigned int _halfsquare = 320; //delay corresponding to a half square travel (for reversal to correct position after colour is sensed)
unsigned int _recogniseColour = 150;//delay corresponding to the time taken to recognise a colour

typedef struct DC_motor { //definition of DC_motor structure
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    char brakemode;		// short or fast decay (brake or coast)
    unsigned int PWMperiod; //base period of PWM cycle
    unsigned char *posDutyHighByte; //PWM duty address for motor +ve side
    unsigned char *negDutyHighByte; //PWM duty address for motor -ve side
} DC_motor;

struct DC_motor motorL, motorR; 		//declare two DC_motor structures 

  
/********************************************//**
 *  Functions to initialise DC motor and set PWM
 *  Initialises PWM and necessary registers for control
 ***********************************************/
void initDCmotorsPWM(unsigned int PWMperiod); 
void setMotorPWM(struct DC_motor *m);
/********************************************//**
 *  Function to control DC motor
 ***********************************************/
void stop(struct DC_motor *mL, struct DC_motor *mR);
void turnLeft(struct DC_motor *mL, struct DC_motor *mR);
void turnRight(struct DC_motor *mL, struct DC_motor *mR);
void rotate180left(struct DC_motor *mL, struct DC_motor *mR);
void reverseHalfSquare(struct DC_motor *mL, struct DC_motor *mR);
/********************************************//**
 *  Sub-functions utilising delays to create relevant movements
 ***********************************************/
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR);
void fullSpeedReverse(struct DC_motor *mL, struct DC_motor *mR);
void Left45(struct DC_motor *mL, struct DC_motor *mR);
void Right45(struct DC_motor *mL, struct DC_motor *mR);


/********************************************//**
 *  Color Functions pointing to their specific movements
 ***********************************************/
void Red_R90(struct DC_motor *mL, struct DC_motor *mR);
void Green_L90(struct DC_motor *mL, struct DC_motor *mR);
void Blue_T180(struct DC_motor *mL, struct DC_motor *mR);
void Yellow_rev1_R90(struct DC_motor *mL, struct DC_motor *mR);
void Pink_rev1_L90(struct DC_motor *mL, struct DC_motor *mR);
void Orange_R135(struct DC_motor *mL, struct DC_motor *mR);
void LightBlue_L135(struct DC_motor *mL, struct DC_motor *mR);
void White(struct DC_motor *mL, struct DC_motor *mR,unsigned int movementCount, volatile unsigned int *movementMemory,volatile float *timerMemory);

#endif
