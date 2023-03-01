
#ifndef _colorfunctions_H
#define	_colorfunctions_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void colour_read_all(struct RGBC_rel *cf);

#endif	/* XC_HEADER_TEMPLATE_H */

