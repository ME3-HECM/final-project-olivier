#ifndef _colorclick_H
#define _colorclick_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  
#define TCS3471_ADDR    0x52
#define TCS3471_ENABLE  0x00
#define TCS3471_ATIME   0x01


/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

void color_write_reg(char reg, char value);

void color_turn_on_led(void);

void color_turn_off_led(void);

/********************************************//**
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 ***********************************************/
void color_writetoaddr(char address, char value);

/********************************************//**
 *  Function to read the red channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Red(void);

/********************************************//**
 *  Function to read the green channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Green(void);

/********************************************//**
 *  Function to read the blue channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Blue(void);

/********************************************//**
 *  Function to read the clear channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Clear(void);



#endif
