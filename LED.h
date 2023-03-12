#ifndef _LED_H
#define _LED_H

#include <xc.h>
#define _XTAL_FREQ 64000000 

void LED_init(void);

//control buggy LED's
#define BrakeLightON LATDbits.LATD4 = 1;
#define BrakeLightOFF LATDbits.LATD4 = 0;
#define IndicatorRightON LATHbits.LATH0 = 1;
#define IndicatorRightOFF LATHbits.LATH0 = 0;
#define IndicatorLeftON LATFbits.LATF0 = 1;
#define IndicatorLeftOFF LATFbits.LATF0 = 0;
#endif 