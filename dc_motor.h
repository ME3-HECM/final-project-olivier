#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>

#define _XTAL_FREQ 64000000

//variable for a forward motion
extern volatile char ForwardFlag;
int _15dleftdelay = 100;
int _15drightdelay = 100;

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
void Left15(struct DC_motor *mL, struct DC_motor *mR);
void Right15(struct DC_motor *mL, struct DC_motor *mR);
void rotate180left(struct DC_motor *mL, struct DC_motor *mR);


void Red_R90(void);
void Green_L90(void);
void Blue_T180(void);
void Yellow_REV1_L90(void);
void Pink_rev1_L90(void);
void Orange_R135(void);
void LightBlue_L135(void);
void White(void);

#endif
