#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "serial.h"
#include <stdio.h>
#include "color.h"
#include "i2c.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    char data[30];
    initUSART4();
    color_click_init();
    I2C_2_Master_Init();
    while (1){ 
        //output color values being read to serial
        Color2String(data,color_read_Red(),color_read_Green(),
                color_read_Blue(),color_read_Clear());
        __delay_ms(1000);
}
}
