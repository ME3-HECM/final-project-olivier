#ifndef _timer_H
#define _timer_H

#include <xc.h>
void Timer0_init(void);
void TimerReset(void);
unsigned int getTimerValue(void);
#endif 
