/*--------------------------------------------------------------------------*/
// Program		: Tilt sensing example by MXD2125  
// Description 	: Display Tilt value sensing by MXD2125 
// Filename		: memsic_lib.h
// C compiler	: WinAVR
/*--------------------------------------------------------------------------*/

#ifndef _MEMSIC_LIB_H_
#define _MEMSIC_LIB_H_

#include <in_out.h>

enum{X,Y};						// Enum axis X and Y

#ifndef Xout
#define Xout in_b(0)
#endif
#ifndef Yout
#define Yout in_b(1)
#endif

void timer1_start() // Config. and Start up timer 0 
{
  	TIFR |= 1<<TOV1; 	//Clear TOV2 / clear
	TCNT1 = 0;			// initial count  	
 	TCCR1B |= (1<<CS11); 	// Prescaler 8,16 MHz,1 MC = 8/16M = 0.5us/count 
}

/*****************************************************************************/
/************************* Function Read angle ****************************/
/*****************************************************************************/
float gforce(char axis)
{
	unsigned int mc=0,i;	// Variable for internal this function 
   float t1,a;
	
	if(axis==X)
	{	
		while(Xout); 		// Detect falling pulse
		while(Xout==0);		// Detect rising pulse
	}
	else
	{
		while(Yout);		// Detect rising pulse
		while(Yout==0);		// Detect rising pulse
	}
	timer1_start();     		// Start timer count
	if(axis==X)
		while(Xout); 		// Detect falling pulse
	else
		while(Yout);		// Detect rising pulse
	TCCR1B = 0;		// Stop timer
	mc = TCNT1;		// Load cycle time count
	t1 = (float)mc*0.0005;		// Calculate High pulse 
	a = ((t1/10.0)-0.5)/0.125;    // Calculate g force
	return(a);		// Return g force
}

int x_angle()
{
	float ax;
	ax = gforce(X);
	if(ax==0.0)
		ax = 0.001;
	return((360.0*asin(ax))/6.283);
}
int y_angle()
{
	float ay;
	ay = gforce(Y);
	if(ay==0.0)
		ay = 0.001;
	return((360.0*asin(ay))/6.283);
}

#endif
