#ifndef _colorfunctions_H
#define	_colorfunctions_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

typedef struct RGBC{             // Define a structure
   float Rf, Gf, Bf, Cf; // Containing the RGBC relative proportions calculated by function
   float h; //Hue variable
   int colourindex; //colour index where each number means a different colour
} RGBC;                 

struct RGBC colorf; //struct 

typedef struct RGB_rel{
    float R, G, B;
}RGB_rel;

struct RGB_rel colorrel;
//an array of colours for reference, can then use indices from olourindex to refer to a colour in this list
//Colourslist[]= {"Red","Green","Blue","Yellow","Pink","Orange,"Light Blue","White"};

/********************************************//**
 *  Function to read all colour values from colour click
 ***********************************************/
void colour_read_all(struct RGBC *cf,struct RGB_rel *rel);
/********************************************//**
 *  Function to convert RGb values to hue
 ***********************************************/
void RGB2Hue(struct RGBC *cf);
/********************************************//**
 *  Function to work out the most likely colour card being looked at based on Hue
 * Values used to determine this are based on calibration
 ***********************************************/
char* Hue2Colour(struct RGBC *cf, struct RGB_rel *rel);
/********************************************************//**
*This functionn takes the colourbeing looked at as a number reference 
*and calls the relevant function
/********************************************************/
void Colour2Action(struct RGBC *cf);
#endif	/* XC_HEADER_TEMPLATE_H */
