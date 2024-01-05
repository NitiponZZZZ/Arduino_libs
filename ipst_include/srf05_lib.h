#ifndef _SHT11_LIB_H_
#define _SHT11_LIB_H_

#include <in_out.h>
#include <sleep.h>

#ifndef ECHO
#define ECHO in_b(2)				// Define recieve pulse pin(SDA)
#endif
#ifndef TRIGGER(x)
#define TRIGGER(x) out_b(3,x)			// Define trigger pulse pin(SCL)
#endif

/*****************************************************************************/
/***************** Function Trigger pulse for start process ******************/
/*****************************************************************************/
void trigger_pulse(void)	  
{
	unsigned char i;			// Variable for counter
	TRIGGER(1);            // Start positive pulse
	for(i=0;i<20;i++)     	// For loop 10 time
	TRIGGER(0);           	// End of positive pulse 	
}
/*****************************************************************************/
/************************* Function Read distance ****************************/
/*****************************************************************************/
unsigned int srf05_dist()
{
   	float mc;	// Variable for internal this function 
	TIFR |= 1<<TOV0; 	//Clear TOV2 / clear
	TCNT0 = 0;			// initial count  	
 	trigger_pulse();	// Send trigger pulse signal	
	while(ECHO);
	while(ECHO==0);		// Detect rising pulse
	TCCR0 |= (1<<CS02)|(1<<CS00); 	// Prescaler 1024,16 MHz,1 MC = 1024/16M = 64us/count 
	while(ECHO); 		// Detect falling pulse
	TCCR0 = 0;
	mc = TCNT0;		// Load cycle time count
	sleep(10);
	return(mc/0.91);	// Return distance cm scale
}

#endif
