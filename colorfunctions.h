#ifndef _colorfunctions_H
#define	_colorfunctions_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

typedef struct RGBC{             // Define a structure
   float Rf, Gf, Bf, Cf; // Containing the RGBC relative proportions calculated by function
   float h;
   int colourindex;
} RGBC;                  // Struct to hold relative values of colors

struct RGBC colorf;

typedef struct RGB_rel{
    float R, G, B;
}RGB_rel;

struct RGB_rel colorrel;
//an array of colours for referwnce, can then use indices from olourindex to refer to a colour in this list
//Colourslist[]= {"Red","Eggshell","Pink","Yellow","Orange","Light Blue","Dark Blue","Green"};


void colour_read_all(struct RGBC *cf,struct RGB_rel *rel);//yuu
void RGB2Hue(struct RGBC *cf);
//This function takes a hue value and works out the most likely colour card being looked at
//Values used to determine this are based on calibration
char* Hue2Colour(struct RGBC *cf, struct RGB_rel *rel);
//This functionn takes the colourbeing looked at as a number reference 
//and calls the relevant function
void Colour2Action(struct RGBC *cf);
#endif	/* XC_HEADER_TEMPLATE_H */
