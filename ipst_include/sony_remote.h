
#ifndef _SONY_REMOTE_H_
#define _SONY_REMOTE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#define KEY_0 137		
#define KEY_1 128
#define KEY_2 129
#define KEY_3 130
#define KEY_4 131
#define KEY_5 132
#define KEY_6 133	
#define KEY_7 134
#define KEY_8 135
#define KEY_9 136
#define KEY_CHANNEL_UP 144
#define KEY_CHANNEL_DOWN 145
#define KEY_VOLUME_UP 146
#define KEY_VOLUME_DOWN 147
#define KEY_POWER 149
#define KEY_AV 165



unsigned int bitCount = 0;
unsigned int _irTmp = 0;
unsigned int _irData = 0;
unsigned int _irCounter=0;
unsigned int _tw=0;

unsigned char sony_ini = 0;
char flag_keep = 0;
char flag_stopExIint = 0;


//char _irstr[15]="";


SIGNAL(SIG_INTERRUPT0) 
{
	
	if((MCUCR & 0x03)==0x02)	// Check Falling edge?
	{
		// Enable Timer 0 for count
		TIFR |= 1<<TOV0; 	//Clear TOV0 / clear
		TCNT0 = 0;	//  = 2700 us  	
  		TIMSK |= 1<<TOIE0; 	//Enable Timer0 Overflow Interrupt
		//TCCR0 = (1<<CS02)|(1<<CS00); 	// Prescaler 1024,16 MHz,1 MC = 1024/16M = 64us/count 
		TCCR0 = (1<<CS02); 	// Prescaler 256,16 MHz,1 MC = 256/16M = 16us/count 
		
		// Change to Rising	detect	
		MCUCR &= 0xFC;
		MCUCR |=0x03;
	}
	else	// For Rising edge detect
	{
		TCCR0 = 0;	// Stop Timer 0
		_tw = TCNT0;	// Get time width
		if(_tw>=152 && _tw<=165)		// Detect start bit
		{
			flag_keep = 1;
			_irTmp = 0;	// Clear before keep IR data
		}
		else
		{
			if(flag_keep==1)
			{
				if(_tw>=32 && _tw<=45)		// Data is "0"
				{
					//_irstr[_irCounter] = '0';		// for test 
					_irTmp = _irTmp>>1;
					 _irCounter++;
					if(_irCounter>11)	// 12 bit DATA/SYSTEM CODE success	
					{
					 	_irCounter = 0;
						_irData = _irTmp;
						flag_keep = 0;
						//_irstr[_irCounter] = 0;	// for test 
						//------------------------- Add for test ------------------------//
						flag_stopExIint = 1;	// Set flag for tell Timer 0 interrupt
						GICR &= ~0x40;	// Disable INT0 interrupt block repeate data
						TCNT0 = 0;			// Clear value before start timer
						TCCR0 = (1<<CS02)|(1<<CS00); 	// Prescaler 1024,16 MHz,1 MC = 1024/16M = 64us/count 
						//---------------------------------------------------------------//
						
					}
				}
				else if(_tw>=65 && _tw<=85) 	// Data is "1"
				{
					_irTmp |= 0x1000;
					//_irstr[_irCounter] = '1';	// for test 
					_irTmp = _irTmp>>1;-
					 _irCounter++;
					if(_irCounter>11)	// 12 bit DATA/SYSTEM CODE success	
					{
						_irCounter = 0;
						_irData = _irTmp;
						flag_keep = 0;
						//_irstr[_irCounter] = 0;	// for test 
						//------------------------- Add for test ------------------------//
						flag_stopExIint = 1;	// Set flag for tell Timer 0 interrupt
						GICR &= ~0x40;	// Disable INT0 interrupt block repeate data
						TCNT0 = 0;			// Clear value before start timer
						TCCR0 = (1<<CS02)|(1<<CS00); 	// Prescaler 1024,16 MHz,1 MC = 1024/16M = 64us/count 
						//---------------------------------------------------------------//
						
					}
				}
				else
				{
					//_irstr[_irCounter] = 'x';		// for test 
					flag_keep = 0;
					 _irCounter = 0;
					 _irTmp = 0;
				}
					
			}
		
		}
		// Change to Falling edge detect	
		MCUCR &= 0xFC;
		MCUCR |=0x02;
	}
	
}
SIGNAL(SIG_OVERFLOW0) // Protect over flow
{
	//------------------------- Add for test ------------------------//
	static unsigned char overFlowTick  = 0;	//
	if(flag_stopExIint==1)
	{
		overFlowTick++;
		if(overFlowTick>10)		// Interval betwen next command
		{
			flag_stopExIint= 0;
			GICR |= 0x40;		// Enable INT0 interrupt
			overFlowTick = 0;
		}
		else
		{
			TCNT0 = 0;			// Clear value before start timer
			TCCR0 = (1<<CS02)|(1<<CS00); 	// Prescaler 1024,16 MHz,1 MC = 1024/16M = 64us/count
		}
	}
	//---------------------------------------------------------------//
	flag_keep = 0;
	_irCounter = 0;
	_irTmp = 0;

}
void sony_init()
{
	MCUCR |= 0x02;	// Config Falling edge detect for INT0(at PD2 pin)
	GICR |= 0x40;	// Enable INT0 interrupt	
	sei();			// Enable global interrupt
}
unsigned int sony_getkey()
{
	unsigned int key=0;
	unsigned int i,j;
	if(sony_ini==0)
	{
		sony_init();
		sony_ini=1;
	}
	key = _irData;
	_irData = 0;
	return(key);
}

#endif
