#ifndef _color_H
#define _color_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  
typedef struct RGBC{             // Define a structure
    int R, G, B, C; // Containing the RGBC values read by the colour click
} RGBC;                  // Struct called RGBC

typedef struct RGBC_rel{             // Define a structure
    int Rf, Gf, Bf, Cf; // Containing the RGBC relative proportions calculated by function
} RGBC_rel;                  // Struct to hold relative values of colors

struct RGBC color; 
struct RGBC_rel colorf;
/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

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

void colour_read_all(struct RGBC *c,struct RGBC_rel *cf);
#endif
