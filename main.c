#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "serial.h"
#include <stdio.h>
#include "colorclick.h"
#include "i2c.h"
#include "colorfunctions.h"
#include "dc_motor.h"
#include "LED.h"
#include "main.h"
#include "interrupts.h"
#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  


void main(void) {
    char data[100];
    //initialisation of modules
    initUSART4();
    color_click_init();
    I2C_2_Master_Init();
    LED_init();
    Interrupts_init();
    Timer0_init();
    unsigned int PWMcycle = 99;
    initDCmotorsPWM(PWMcycle);
    //setting the motor characteristics
    motorL.power=0; 						//zero power to start
    motorL.direction=1; 					//set default motor direction
    motorL.brakemode=1;						// brake mode (slow decay)
    motorL.posDutyHighByte=(unsigned char *)(&CCPR1H);  //store address of CCP1 duty high byte
    motorL.negDutyHighByte=(unsigned char *)(&CCPR2H);  //store address of CCP2 duty high byte
    motorL.PWMperiod=PWMcycle; 			//store PWMperiod for motor (value of T2PR in this case)
    
    motorR.power=0; 						//zero power to start
    motorR.direction=1; 					//set default motor direction
    motorR.brakemode=1;						// brake mode (slow decay)
    motorR.posDutyHighByte=(unsigned char *)(&CCPR3H);  //store address of CCP3 duty high byte
    motorR.negDutyHighByte=(unsigned char *)(&CCPR4H);  //store address of CCP4 duty high byte
    motorR.PWMperiod=PWMcycle; 			//store PWMperiod for motor (value of T2PR in this case)
    
    TRISFbits.TRISF2=1;//set the RF2 button to be an input 
    ANSELFbits.ANSELF2=0;//makes the button digital (0 or 1)
    while(PORTFbits.RF2){//wait for the RF2 button press
    }
    __delay_ms(500);
    TimerReset();//reset the timer
    char wall=0;//set the wall condition to 0

    ClickLEDOn(1);//set the clicker LED initially to off
    
    char buf[20];
    while (!retracingDone){ //run this code until the white function is called 
        fullSpeedAhead(&motorL,&motorR);//move the buggy forwards
        //wait to run into a wall
        while (!wall){
            colour_read_all(&colorf,&colorrel);//read RGB values from colour clicker
             //when in contact with a wall or card a lot less light is received
             //by sensors so all sensor values fall
             if (colorf.Cf>300)//wait for the clear value to be under a certain threshold (dark)
             {
//                if (maxTime==1){//if the maximum time between actions (8 seconds) has been reached, perform the return home function
//                //here we assume a wall has been reached but the time between actions has exceeded 8 seconds and so the buggy must return home  
//                //note: since the timer is reset every time an action is performed the timer does not need to be reset here
//                    memoryUpdate(&colorf,movementCount,movementMemory,timerMemory);//update the memory function
//                    White(&motorL,&motorR,movementCount,movementMemory,timerMemory);//perform the return home function 
//                }
            memoryUpdateTime(movementCount,timerMemory);//update the time taken for action to occur corresponding to the movement
            //flag that a wall has been detected
            wall=1;
            __delay_ms(100);//this delay makes sure that the colour is constant when being read
            stop(&motorL,&motorR);//stop the buggy
            }
        }
        colour_read_all(&colorf,&colorrel);//read the colours from the colour click
        wall=0;
        RGB2Hue(&colorf);//takes the RGB values and outputs hue 
        Hue2Colour(&colorf,&colorrel);//takes the hue and outputs the colour
        memoryUpdateMovement(&colorf,movementCount,movementMemory);//update the memory function
        Colour2Action(&colorf);//perform the action
        if (colorf.colourindex == 7)//if the white function is called
        {
            while(!retracingDone){}//wait until the retracing is done before resetting the timer as it might mess up the white function
        }
        TimerReset();//reset the timer in order to have time between actions
        movementCount++; //increment the movement coun
    }
}