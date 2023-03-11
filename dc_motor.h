#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>

#define _XTAL_FREQ 64000000

//variable for a forward motion
extern volatile char ForwardFlag = 1;
int _45dleftdelay = 300; //delay corresponging to a 15 degree turn left
int _45drightdelay = 300; //delay corresponding to a 15 degree turn right
int _1square = 2000; //delay corresponding to 1 square travel
int _halfsquare = 1000; //delay corresponding to a half square travel (for reversal to correct position after colour is sensed)

typedef struct DC_motor { //definition of DC_motor structure
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    char brakemode;		// short or fast decay (brake or coast)
    unsigned int PWMperiod; //base period of PWM cycle
    unsigned char *posDutyHighByte; //PWM duty address for motor +ve side
    unsigned char *negDutyHighByte; //PWM duty address for motor -ve side
} DC_motor;
struct DC_motor motorL, motorR; 		//declare two DC_motor structures 

  
//function prototypes
void initDCmotorsPWM(unsigned int PWMperiod); // function to setup PWM
void setMotorPWM(struct DC_motor *m);
void stop(struct DC_motor *mL, struct DC_motor *mR);
void turnLeft(struct DC_motor *mL, struct DC_motor *mR);
void turnRight(struct DC_motor *mL, struct DC_motor *mR);
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR);
void fullSpeedReverse(struct DC_motor *mL, struct DC_motor *mR);
void Left45(struct DC_motor *mL, struct DC_motor *mR);
void Right45(struct DC_motor *mL, struct DC_motor *mR);
void rotate180left(struct DC_motor *mL, struct DC_motor *mR);


void Red_R90(struct DC_motor *mL, struct DC_motor *mR);
void Green_L90(struct DC_motor *mL, struct DC_motor *mR);
void Blue_T180(struct DC_motor *mL, struct DC_motor *mR);
void Yellow_REV1_R90(struct DC_motor *mL, struct DC_motor *mR);
void Pink_rev1_L90(struct DC_motor *mL, struct DC_motor *mR);
void Orange_R135(struct DC_motor *mL, struct DC_motor *mR);
void LightBlue_L135(struct DC_motor *mL, struct DC_motor *mR);
void White(struct DC_motor *mL, struct DC_motor *mR);

#endif
