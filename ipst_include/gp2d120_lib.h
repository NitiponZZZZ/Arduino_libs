/*--------------------------------------------------------------------------*/
// Filename			: gp2d120_lib.h
// C compiler		: avr-gcc
/*--------------------------------------------------------------------------*/

#ifndef _GP2D120_LIB_H_
#define _GP2D120_LIB_H_

#include <analog.h>
#define m 2933
#define b 20
#define k 1
unsigned int getdist(char adc_ch)
{
	unsigned int dist = 0;
   dist = (m/(analog(adc_ch)+b)) - k;
	return(dist);
}

#endif
