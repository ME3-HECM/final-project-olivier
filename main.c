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
#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    char data[100];
    initUSART4();
    color_click_init();
    I2C_2_Master_Init();
    LED_init();
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
    
    TRISFbits.TRISF2=1;
    ANSELFbits.ANSELF2=0;
    char wall=0;
    ClickLEDOn(0);
    unsigned int movementCount = 0;

    while (1){ 
        Left45(&motorL,&motorR);
        Sleep();
    }
}