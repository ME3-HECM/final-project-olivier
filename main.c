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
    initUSART4();
    color_click_init();
    I2C_2_Master_Init();
    LED_init();
    Interrupts_init();
    Timer0_init();
    unsigned int PWMcycle = 99;
    initDCmotorsPWM(PWMcycle);

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
    char wall=0;//set the wall condition to 0

    ClickLEDOn(0);//set the clicker LED initially to off

    unsigned int movementCount = 0;//define the movement counter

    while (1){ //main function
        fullSpeedAhead(&motorL,&motorR);//move the buggy forwards
        if (maxTime==1){//if the maximum time between actions (8 seconds) has been reached 
            stop(&motorL,&motorR);//testing to see if the timer interrupt will work currently it overflows after 8 seconds 
            __delay_ms(3000);
        }
        //wait to run into a wall
        while (!wall){
            colour_read_all(&colorf);//read RGB values from colour clicker
            Color2String(data,&colorf);//output a string of the colour
             //when in contact with a wall or card a lot less light is received
             //by sensors so all sensor values fall
             if (colorf.Cf<100)//wait for the clear value to be under a certain threshold (dark)
             {
                 //flag that a wall has been detected
                 wall=1;
                 ClickLEDOn(1);//turn on the LED to read the wall colour
                 stop(&motorL,&motorR);//stop the buggy 
                 __delay_ms(2000);//this delay makes sure that the colour is constant when being read
             }
        }
        colour_read_all(&colorf);//read the colours from the colour click
        RGB2Hue(&colorf);//takes the RGB values and outputs hue 
        Hue2Colour(&colorf);//takes the hue and outputs the colour
        Colour2Action(&colorf);//perform the action
        
        //output color values being read to serial
        Color2String(data,&colorf);
        __delay_ms(1000);
        wall=0;
        ClickLEDOn(0);
    }
}