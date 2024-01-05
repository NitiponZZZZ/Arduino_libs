
#ifndef _SEGMENT_H_
#define _SEGMENT_H_
//-------------------------------------------------------------------------------------//
// Library for Display data on 7-Segment by Timer 2 interrupt every 5 ms
//-------------------------------------------------------------------------------------//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <in_out.h>
//------------------------ Default pin for segment drive ------------------------------//
#ifndef DIGIT5(x)
#define DIGIT5(x) out_b(0,x)	// Define pin for drive on digit1( or DIGIT4)
#endif  

#ifndef DIGIT4(x)
#define DIGIT4(x) out_b(4,x)	// Define pin for drive on digit1( or DIGIT4)
#endif  

#ifndef DIGIT3(x)
#define DIGIT3(x) out_b(3,x)	// Define pin for drive on digit2( or DIGIT3)
#endif  

#ifndef DIGIT2(x)
#define DIGIT2(x) out_b(2,x)	// Define pin for drive on digit3( or DIGIT2)
#endif   

#ifndef DIGIT1(x)
#define DIGIT1(x) out_b(1,x)	// Define pin for drive on digit4( or DIGIT1)
#endif  
//-------------------------------------------------------------------------------------//	
#ifndef _DOT
#define _DOT 0x01	// Define dot for display
#endif  
#ifndef NONE
#define NONE 0	// Define dot for display
#endif  
#ifndef DOT1
#define DOT1 1	// Define dot for display
#endif  
#ifndef DOT2
#define DOT2 2	// Define dot for display
#endif  
#ifndef DOT3
#define DOT3 3	// Define dot for display
#endif  
#ifndef DOT4
#define DOT4 4	// Define dot for display
#endif  
#define SEGMENT segment

char round_count = 3; 
char seg_i=0;			// For check digit 1-4(seg_i=0==> display digit1) 
						// (seg_i=1==> display digit2)
						// (seg_i=2==> display digit3)
						// (seg_i=3==> display digit4)
char segment_ini=0,led_ini=0 ;		// For check first time start up timer 2 	

unsigned int segment_dat=0;	// Keep value for display on 7-Segment(4 digit)
							// interval value is(0-9999)
unsigned char led_dat=0;	// Keep value for display on 7-Segment(4 digit)
							// interval value is(0-9999)

							// if over then display "----" on 7-Segment	
unsigned char _segment_dot=0;	// Dot status for display  on 7-Segment(4 digit)


unsigned char  num[10]=	{	0xFC,	// Code number 0
							0x60,	// Code number 1	
							0xDA,	// Code number 2
							0xF2,	// Code number 3
							0x66,	// Code number 4
							0xB6,	// Code number 5
							0xBE,	// Code number 6
							0xE0,	// Code number 7
							0xFE,	// Code number 8
							0xF6,	// Code number 9	
						};



SIGNAL (SIG_OVERFLOW2) // Interval 5 ms
{
	TCNT2 = 178;	// Reload interval 5 ms(TCNT2 = 178)  
	
		if(seg_i>round_count)	// Counter loop
		{
			seg_i=0;
		}
		
		if(segment_ini)
		{
		
			if(led_ini==1)
			{
				DIGIT5(0);	// Not arrow LED display on task SEGMENT
			}
			if(seg_i==0)		// Digit 1
			{
				if(segment_dat>9999)
					PORTC = 0x02;	// Display "-" because overflow
				else
				{
					if(_segment_dot==1)
						PORTC = num[segment_dat%10] | _DOT;
					else
						PORTC = num[segment_dat%10];
				}
				DIGIT4(1);
				DIGIT3(0);
				DIGIT2(0);
				DIGIT1(0);
			}
			else if(seg_i==1)	// Digit 2
			{
				if(segment_dat>9999)
					PORTC = 0x02;	// Display "-" because overflow
				else
				{
					if(_segment_dot==2)
						PORTC = num[(segment_dat%100)/10] | _DOT;
					else
						PORTC = num[(segment_dat%100)/10];
				}
				DIGIT4(0);
				DIGIT3(1);
				DIGIT2(0);
				DIGIT1(0);
				
			}
			else if(seg_i==2)	// Digit 3
			{
				if(segment_dat>9999)
					PORTC = 0x02;	// Display "-" because overflow
				else
				{
					if(_segment_dot==3)
						PORTC = num[(segment_dat%1000)/100] | _DOT;
					else
						PORTC = num[(segment_dat%1000)/100];
				}
				DIGIT4(0);
				DIGIT3(0);
				DIGIT2(1);
				DIGIT1(0);
			}
			else if(seg_i==3)	// Digit 4
			{
				if(segment_dat>9999)
					PORTC = 0x02;	// Display "-" because overflow
				else
				{
					if(_segment_dot==4)
						PORTC = num[segment_dat/1000] | _DOT;
					else
						PORTC = num[segment_dat/1000];
				}
				DIGIT4(0);
				DIGIT3(0);
				DIGIT2(0);
				DIGIT1(1);
			}
			else if(seg_i==4) // Add on for test
			{
		
				PORTC = 0;
				DIGIT4(0);
				DIGIT3(0);
				DIGIT2(0);
				DIGIT1(0);
			}
	}	
	
	if(seg_i==4 && led_ini==1)	// Led display
	{
		
		PORTC = led_dat;
		DIGIT4(0);
		DIGIT3(0);
		DIGIT2(0);
		DIGIT1(0);
		DIGIT5(1);
	}
	
	seg_i++;
		
}

void segment_init() // Config. and Start up timer 0 
{
   	DDRC = 0xFF; // Port C output only
	//DDRB = 0x1F; // Bit control set output
	//DDRB = 0;	
	TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20); 	// Prescaler 1024,16 MHz,1 MC = 1024/16M = 64us/count 
	TIFR |= 1<<TOV2; 	//Clear TOV2 / clear
	TIMSK |= 1<<TOIE2; 	//Enable Timer2 Overflow Interrupt
	TCNT2 = 178;	// Interval 5 ms  	
    sei();			// Enable all interrupt
}
void segment(unsigned int val)
{
	if(segment_ini==0)
	{
		segment_ini=1;
		segment_init();
	}
	segment_dat = val;
}
void segment_dot(unsigned char _dot_val)
{
	if(segment_ini==0)
	{
		segment_ini=1;
		segment_init();
	}
	_segment_dot = _dot_val;
}
void segment_off()
{
	if(led_ini==0)
	{
		TIMSK &= ~(1<<TOIE2); 	//Disable Timer2 Overflow Interrupt(Disable 7-segment for display)
	}
	round_count = 4;	// Count for led
	segment_ini=0;	// Restore before start up 7-segment
	DIGIT4(0);	// Disable digit 1
	DIGIT3(0);	// Disable digit 2
	DIGIT2(0);	// Disable digit 3
	DIGIT1(0);	// Disable digit 4
}
void led8(unsigned char _dat)
{
	round_count = 4;	// Count for led
	if(led_ini==0)
	{
		led_ini=1;
		segment_init();
	}
	led_dat = _dat;	
}
void led8_off()
{
	if(segment_ini==0)
	{
		TIMSK &= ~(1<<TOIE2); 	//Disable Timer2 Overflow Interrupt(Disable 7-segment for display)
	}
	round_count = 3;	// Count for Segment 1-4 only
	led_ini = 0;
	DIGIT5(0);
}

#endif
