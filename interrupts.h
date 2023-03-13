#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>
#include "main.h"

#define _XTAL_FREQ 64000000

void Interrupts_init(void);
void __interrupt(high_priority) HighISR();

#endif
