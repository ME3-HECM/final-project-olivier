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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    char data[80];
    initUSART4();
    color_click_init();
    I2C_2_Master_Init();

    char wall=0;
    while (1){ 
        //wait to run into a wall
        while (!wall){
            colour_read_all(&colorf);
            Color2String(data,&colorf);
             
             //when in contact with a wall or card a lot less light is received
             //by sensors so all sensor values fall
             if (colorf.Cf<1500)
             {
                 //flag that a wall has been detected
                 wall=1;
                 ClickLEDOn(1);
                 __delay_ms(1000);
             }
        }
        colour_read_all(&colorf);
        RGB2Hue(&colorf);
        //Colour2Action(&colorf);
        //output color values being read to serial
        Color2String(data,&colorf);
        __delay_ms(1000);
        wall=0;
        ClickLEDOn(0);
}
}
