#ifndef _TIMER_H_
#define _TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

/**********************************************************************************/
/*************************** Timer 0  Interrupt ***********************************/
/*************************** Interval 1 ms ****************************************/
/**********************************************************************************/

unsigned long _ms=0;

SIGNAL (SIG_OVERFLOW0) // Interval 1 ms
{
	TCNT0 = 6;	// Interval 1 ms  
	_ms++;
	
}
void timer_start(void) // Config. and Start up timer 0 
{
   
	TCCR0 = (1<<CS01)|(1<<CS00); 	// Prescaler 64,16 MHz,1 MC = 64/16M = 4us/count 
	TIFR |= 1<<TOV0; 	//Clear TOV0 / clear
	TIMSK |= 1<<TOIE0; 	//Enable Timer0 Overflow Interrupt
	TCNT0 = 6;	// Interval 1 ms  	
    
	sei();	// Enable all interrupt
	_ms = 0;
}
void timer_stop()
{
	TCCR0 = 0;		// Stop timer and 
	TCNT0 = 0;
	TIMSK &= ~_BV(TOIE0);	// Clear bit TOIE0
	_ms = 0;		// Clear time
}
void timer_pause()
{
	TCCR0 = 0;		// Stop timer and not clear time
}
void timer_resume()
{
	TCCR0 = (1<<CS01)|(1<<CS00); 	// Prescaler 64,16 MHz,1 MC = 64/16M = 4us/count 
}
unsigned long msec()
{
	return(_ms);	
} 
unsigned long sec()
{ 
	return(_ms/1000);	
} 

#endif
