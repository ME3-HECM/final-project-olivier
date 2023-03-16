#ifndef _SERIAL_H
#define _SERIAL_H

#include <xc.h>
#include "colorfunctions.h"
#include "colorclick.h"
#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

//basic EUSART funcitons
void initUSART4(void);
char getCharSerial4(void);
void sendCharSerial4(char charToSend);
void sendStringSerial4(char *string);

/********************************************//**
 *  Function to output RGB values and predicted colour
 ***********************************************/
void Color2String(char *buf,struct RGBC *cf,struct RGB_rel *rel);
/********************************************//**
 *  Function to output relative RGB values to serial
 ***********************************************/
void RelColor2String(char *buf,struct RGB_rel *rel);
#endif
